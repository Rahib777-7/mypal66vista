"use strict";

async function runTest(options) {
  options.neutral = [0xaa, 0xaa, 0xaa];

  let html = `
    <!DOCTYPE html>
    <html lang="en">
    <head><meta charset="UTF-8"></head>
    <body style="background-color: rgb(${options.color})">
      <!-- Fill most of the image with a neutral color to test edge-to-edge scaling. -->
      <div style="position: absolute;
                  left: 2px;
                  right: 2px;
                  top: 2px;
                  bottom: 2px;
                  background: rgb(${options.neutral});"></div>
    </body>
    </html>
  `;

  let url = `data:text/html,${encodeURIComponent(html)}`;
  let tab = await BrowserTestUtils.openNewForegroundTab(gBrowser, url, true);

  tab.linkedBrowser.fullZoom = options.fullZoom;

  async function background(options) {
    browser.test.log(
      `Test color ${options.color} at fullZoom=${options.fullZoom}`
    );

    try {
      let [tab] = await browser.tabs.query({
        currentWindow: true,
        active: true,
      });

      let [jpeg, png, ...pngs] = await Promise.all([
        browser.tabs.captureTab(tab.id, { format: "jpeg", quality: 95 }),
        browser.tabs.captureTab(tab.id, { format: "png", quality: 95 }),
        browser.tabs.captureTab(tab.id, { quality: 95 }),
        browser.tabs.captureTab(tab.id),
      ]);

      browser.test.assertTrue(
        pngs.every(url => url == png),
        "All PNGs are identical"
      );

      browser.test.assertTrue(
        jpeg.startsWith("data:image/jpeg;base64,"),
        "jpeg is JPEG"
      );
      browser.test.assertTrue(
        png.startsWith("data:image/png;base64,"),
        "png is PNG"
      );

      let promises = [jpeg, png].map(
        url =>
          new Promise(resolve => {
            let img = new Image();
            img.src = url;
            img.onload = () => resolve(img);
          })
      );

      [jpeg, png] = await Promise.all(promises);
      let tabDims = `${tab.width}\u00d7${tab.height}`;

      let images = { jpeg, png };
      for (let format of Object.keys(images)) {
        let img = images[format];

        let dims = `${img.width}\u00d7${img.height}`;
        browser.test.assertEq(
          tabDims,
          dims,
          `${format} dimensions are correct`
        );

        let canvas = document.createElement("canvas");
        canvas.width = img.width;
        canvas.height = img.height;
        canvas.mozOpaque = true;

        let ctx = canvas.getContext("2d");
        ctx.drawImage(img, 0, 0);

        // Check the colors of the first and last pixels of the image, to make
        // sure we capture the entire frame, and scale it correctly.
        let coords = [
          { x: 0, y: 0, color: options.color },
          { x: img.width - 1, y: img.height - 1, color: options.color },
          {
            x: (img.width / 2) | 0,
            y: (img.height / 2) | 0,
            color: options.neutral,
          },
        ];

        for (let { x, y, color } of coords) {
          let imageData = ctx.getImageData(x, y, 1, 1).data;

          if (format == "png") {
            browser.test.assertEq(
              `rgba(${color},255)`,
              `rgba(${[...imageData]})`,
              `${format} image color is correct at (${x}, ${y})`
            );
          } else {
            // Allow for some deviation in JPEG version due to lossy compression.
            const SLOP = 3;

            browser.test.log(
              `Testing ${format} image color at (${x}, ${y}), have rgba(${[
                ...imageData,
              ]}), expecting approx. rgba(${color},255)`
            );

            browser.test.assertTrue(
              Math.abs(color[0] - imageData[0]) <= SLOP,
              `${format} image color.red is correct at (${x}, ${y})`
            );
            browser.test.assertTrue(
              Math.abs(color[1] - imageData[1]) <= SLOP,
              `${format} image color.green is correct at (${x}, ${y})`
            );
            browser.test.assertTrue(
              Math.abs(color[2] - imageData[2]) <= SLOP,
              `${format} image color.blue is correct at (${x}, ${y})`
            );
            browser.test.assertEq(
              255,
              imageData[3],
              `${format} image color.alpha is correct at (${x}, ${y})`
            );
          }
        }
      }

      browser.test.notifyPass("captureTab");
    } catch (e) {
      browser.test.fail(`Error: ${e} :: ${e.stack}`);
      browser.test.notifyFail("captureTab");
    }
  }

  let extension = ExtensionTestUtils.loadExtension({
    manifest: {
      permissions: ["<all_urls>"],
    },

    background: `(${background})(${JSON.stringify(options)})`,
  });

  await extension.startup();

  await extension.awaitFinish("captureTab");

  await extension.unload();

  BrowserTestUtils.removeTab(tab);
}

add_task(async function testCaptureTab() {
  await runTest({ color: [0, 0, 0], fullZoom: 1 });

  await runTest({ color: [0, 0, 0], fullZoom: 2 });

  await runTest({ color: [0, 0, 0], fullZoom: 0.5 });

  await runTest({ color: [255, 255, 255], fullZoom: 1 });
});

add_task(async function testCaptureTabPermissions() {
  let extension = ExtensionTestUtils.loadExtension({
    manifest: {
      permissions: ["tabs"],
    },

    background() {
      browser.test.assertEq(
        undefined,
        browser.tabs.captureTab,
        'Extension without "<all_urls>" permission should not have access to captureTab'
      );
      browser.test.notifyPass("captureTabPermissions");
    },
  });

  await extension.startup();

  await extension.awaitFinish("captureTabPermissions");

  await extension.unload();
});
