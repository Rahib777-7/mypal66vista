"use strict";

/* globals chrome */

async function testPermission(options) {
  function background(bgOptions) {
    browser.test.sendMessage("typeof-namespace", {
      browser: typeof browser[bgOptions.namespace],
      chrome: typeof chrome[bgOptions.namespace],
    });
  }

  let extensionDetails = {
    background: `(${background})(${JSON.stringify(options)})`,
  };

  let extension = ExtensionTestUtils.loadExtension(extensionDetails);

  await extension.startup();

  let types = await extension.awaitMessage("typeof-namespace");
  equal(
    types.browser,
    "undefined",
    `Type of browser.${options.namespace} without manifest entry`
  );
  equal(
    types.chrome,
    "undefined",
    `Type of chrome.${options.namespace} without manifest entry`
  );

  await extension.unload();

  extensionDetails.manifest = options.manifest;
  extension = ExtensionTestUtils.loadExtension(extensionDetails);

  await extension.startup();

  types = await extension.awaitMessage("typeof-namespace");
  equal(
    types.browser,
    "object",
    `Type of browser.${options.namespace} with manifest entry`
  );
  equal(
    types.chrome,
    "object",
    `Type of chrome.${options.namespace} with manifest entry`
  );

  await extension.unload();
}

add_task(async function test_browserAction() {
  await testPermission({
    namespace: "browserAction",
    manifest: {
      browser_action: {},
    },
  });
});

add_task(async function test_pageAction() {
  await testPermission({
    namespace: "pageAction",
    manifest: {
      page_action: {},
    },
  });
});
