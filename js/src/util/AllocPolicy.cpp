/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "js/AllocPolicy.h"

#include "vm/JSContext.h"

using namespace js;

void* TempAllocPolicy::onOutOfMemory(arena_id_t arenaId,
                                     AllocFunction allocFunc, size_t nbytes,
                                     void* reallocPtr) {
  return cx_->onOutOfMemory(allocFunc, arenaId, nbytes, reallocPtr);
}

void TempAllocPolicy::reportAllocOverflow() const {
  ReportAllocationOverflow(cx_);
}
