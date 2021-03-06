/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

// Tests if the JSTerm sandbox is updated when the user navigates from one
// domain to another, in order to avoid permission denied errors with a sandbox
// created for a different origin. See Bug 664688.

"use strict";

const BASE_URI =
  "browser/devtools/client/webconsole/" + "test/mochitest/test-console.html";
const TEST_URI1 = "http://example.com/" + BASE_URI;
const TEST_URI2 = "http://example.org/" + BASE_URI;

add_task(async function() {
  pushPref("devtools.webconsole.persistlog", false);

  const hud = await openNewTabAndConsole(TEST_URI1);

  let onMessages = waitForMessages({
    hud,
    messages: [{ text: "window.location.href" }, { text: TEST_URI1 }],
  });

  hud.jsterm.execute("window.location.href");

  info("wait for window.location.href");
  await onMessages;

  // load second url
  BrowserTestUtils.loadURI(gBrowser.selectedBrowser, TEST_URI2);
  await BrowserTestUtils.browserLoaded(gBrowser.selectedBrowser);

  ok(!findMessage(hud, "Permission denied"), "no permission denied errors");

  onMessages = waitForMessages({
    hud,
    messages: [{ text: "window.location.href" }, { text: TEST_URI2 }],
  });

  hud.ui.clearOutput();
  hud.jsterm.execute("window.location.href");

  info("wait for window.location.href after page navigation");
  await onMessages;

  ok(!findMessage(hud, "Permission denied"), "no permission denied errors");

  // Navigation clears messages. Wait for that clear to happen before
  // continuing the test or it might destroy messages we wait later on (Bug
  // 1270234).
  const cleared = hud.ui.once("messages-cleared");

  gBrowser.goBack();

  info("Waiting for messages-cleared event due to navigation");
  await cleared;

  info("Messages cleared after navigation; checking location");

  onMessages = waitForMessages({
    hud,
    messages: [{ text: "window.location.href" }, { text: TEST_URI1 }],
  });

  hud.jsterm.execute("window.location.href");

  info("wait for window.location.href after goBack()");
  await onMessages;

  ok(!findMessage(hud, "Permission denied"), "no permission denied errors");
});
