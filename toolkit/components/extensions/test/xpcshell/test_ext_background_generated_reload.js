"use strict";

add_task(async function test_reload_generated_background_page() {
  let extension = ExtensionTestUtils.loadExtension({
    background() {
      if (location.hash !== "#firstrun") {
        browser.test.sendMessage("first run");
        location.hash = "#firstrun";
        browser.test.assertEq("#firstrun", location.hash);
        location.reload();
      } else {
        browser.test.notifyPass("second run");
      }
    },
  });

  await extension.startup();
  await extension.awaitMessage("first run");
  await extension.awaitFinish("second run");

  await extension.unload();
});
