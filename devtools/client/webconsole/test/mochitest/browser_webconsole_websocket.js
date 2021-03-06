/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

// Check that WebSocket connection failure messages are displayed. See Bug 603750.

const TEST_URI =
  "http://example.com/browser/devtools/client/webconsole/" +
  "test/mochitest/test-websocket.html";
const TEST_URI2 =
  "data:text/html;charset=utf-8,Web Console test for " +
  "bug 603750: Web Socket errors";

add_task(async function() {
  const hud = await openNewTabAndConsole(TEST_URI2);

  BrowserTestUtils.loadURI(gBrowser.selectedBrowser, TEST_URI);

  await waitFor(() => findMessage(hud, "ws://0.0.0.0:81"), null, 100);
  await waitFor(() => findMessage(hud, "ws://0.0.0.0:82"), null, 100);
  ok(true, "WebSocket error messages are displayed in the console");
});
