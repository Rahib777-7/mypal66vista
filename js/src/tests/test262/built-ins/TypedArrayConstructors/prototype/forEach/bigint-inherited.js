// |reftest| skip-if(!this.hasOwnProperty('BigInt')) -- BigInt is not enabled unconditionally
// Copyright (C) 2016 the V8 project authors. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.
/*---
esid: sec-%typedarray%.prototype.foreach
description: >
  _TypedArray_.prototype has no own property "forEach"
includes: [testBigIntTypedArray.js]
features: [BigInt, TypedArray]
---*/

testWithBigIntTypedArrayConstructors(function(TA) {
  assert.sameValue(TA.prototype.hasOwnProperty("forEach"), false);
});

reportCompare(0, 0);