/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

const TEST_URI = `data:text/html,
<main>
  <ul>
    <li>First</li>
    <li>Second</li>
  </ul>
  <aside>Sidebar</aside>
</main>
`;

add_task(async function() {
  // Run test with legacy JsTerm
  await pushPref("devtools.webconsole.jsterm.codeMirror", false);
  await performTests();
  // And then run it with the CodeMirror-powered one.
  await pushPref("devtools.webconsole.jsterm.codeMirror", true);
  await performTests();
});

async function performTests() {
  const hud = await openNewTabAndConsole(TEST_URI);
  const jsterm = hud.jsterm;

  let onMessage = waitForMessage(hud, "<main>");
  jsterm.execute("$('main')");
  let message = await onMessage;
  ok(message, "`$('main')` worked");

  onMessage = waitForMessage(hud, "<li>");
  jsterm.execute("$('main > ul > li')");
  message = await onMessage;
  ok(message, "`$('main > ul > li')` worked");

  onMessage = waitForMessage(hud, "LI");
  jsterm.execute("$('main > ul > li').tagName");
  message = await onMessage;
  ok(message, "`$` result can be used right away");

  onMessage = waitForMessage(hud, "null");
  jsterm.execute("$('div')");
  message = await onMessage;
  ok(message, "`$('div')` does return null");
}
