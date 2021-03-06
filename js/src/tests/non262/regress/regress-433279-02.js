/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 433279;
var summary = 'Do not assert: pn != tc->parseContext->nodeList';
var actual = 'No Crash';
var expect = 'No Crash';

//-----------------------------------------------------------------------------
test();
//-----------------------------------------------------------------------------

function test()
{
  printBugNumber(BUGNUMBER);
  printStatus (summary);
 
  try
  {
    eval('var {a} = b; c(d + 1);');
  }
  catch(ex)
  {
  }

  reportCompare(expect, actual, summary);
}
