/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * The origin of this IDL file is
 * http://dom.spec.whatwg.org/#domerror
 *
 * Copyright © 2012 W3C® (MIT, ERCIM, Keio), All Rights Reserved. W3C
 * liability, trademark and document use rules apply.
 */

[Constructor(DOMString name, optional DOMString message = ""),
 Exposed=(Window,Worker)]
interface DOMError {
  [Constant, UseCounter]
  readonly attribute DOMString name;

  [Constant, UseCounter]
  readonly attribute DOMString message;
};
