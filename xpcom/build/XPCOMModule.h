/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef XPCOMModule_h
#define XPCOMModule_h

#include "nsID.h"

class nsISupports;

nsresult nsThreadManagerGetSingleton(nsISupports* aOuter, const nsIID& aIID,
                                     void** aInstancePtr);

nsresult nsLocalFileConstructor(nsISupports* aOuter, const nsIID& aIID,
                                void** aInstancePtr);

extern nsresult nsStringInputStreamConstructor(nsISupports*, const nsID&,
                                               void**);

#endif  // XPCOMModule_h
