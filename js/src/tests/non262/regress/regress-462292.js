/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 462292;
var summary = 'Do not assert: pn->pn_op == JSOP_CALL || pn->pn_op == JSOP_EVAL';
var actual = '';
var expect = '';


//-----------------------------------------------------------------------------
test();
//-----------------------------------------------------------------------------

function test()
{
  printBugNumber(BUGNUMBER);
  printStatus (summary);
 
  try
  {
    [].apply() = 1;
  }
  catch(ex)
  {
  }

  reportCompare(expect, actual, summary);
}
