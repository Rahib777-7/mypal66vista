/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_ServoElementSnapshotTable_h
#define mozilla_ServoElementSnapshotTable_h

#include "mozilla/dom/Element.h"
#include "nsClassHashtable.h"
#include "nsHashKeys.h"
#include "ServoElementSnapshot.h"

namespace mozilla {

class ServoElementSnapshotTable
    : public nsClassHashtable<nsRefPtrHashKey<dom::Element>,
                              ServoElementSnapshot> {};

}  // namespace mozilla

#endif
