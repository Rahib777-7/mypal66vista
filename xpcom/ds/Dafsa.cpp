/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Dafsa.h"

#include "mozilla/Assertions.h"
#include "nsAString.h"

const int mozilla::Dafsa::kKeyNotFound = -1;

// Note the DAFSA implementation was lifted from eTLD code in Chromium that was
// originally lifted from Firefox.

// Read next offset from pos.
// Returns true if an offset could be read, false otherwise.
static bool GetNextOffset(const unsigned char** pos, const unsigned char* end,
                          const unsigned char** offset) {
  if (*pos == end) return false;

  // When reading an offset the byte array must always contain at least
  // three more bytes to consume. First the offset to read, then a node
  // to skip over and finally a destination node. No object can be smaller
  // than one byte.
  MOZ_ASSERT(*pos + 2 < end);
  size_t bytes_consumed;
  switch (**pos & 0x60) {
    case 0x60:  // Read three byte offset
      *offset += (((*pos)[0] & 0x1F) << 16) | ((*pos)[1] << 8) | (*pos)[2];
      bytes_consumed = 3;
      break;
    case 0x40:  // Read two byte offset
      *offset += (((*pos)[0] & 0x1F) << 8) | (*pos)[1];
      bytes_consumed = 2;
      break;
    default:
      *offset += (*pos)[0] & 0x3F;
      bytes_consumed = 1;
  }
  if ((**pos & 0x80) != 0) {
    *pos = end;
  } else {
    *pos += bytes_consumed;
  }
  return true;
}

// Check if byte at offset is last in label.
static bool IsEOL(const unsigned char* offset, const unsigned char* end) {
  MOZ_ASSERT(offset < end);
  return (*offset & 0x80) != 0;
}

// Check if byte at offset matches first character in key.
// This version matches characters not last in label.
static bool IsMatch(const unsigned char* offset, const unsigned char* end,
                    const char* key) {
  MOZ_ASSERT(offset < end);
  return *offset == *key;
}

// Check if byte at offset matches first character in key.
// This version matches characters last in label.
static bool IsEndCharMatch(const unsigned char* offset,
                           const unsigned char* end, const char* key) {
  MOZ_ASSERT(offset < end);
  return *offset == (*key | 0x80);
}

// Read return value at offset.
// Returns true if a return value could be read, false otherwise.
static bool GetReturnValue(const unsigned char* offset,
                           const unsigned char* end, int* return_value) {
  MOZ_ASSERT(offset < end);
  if ((*offset & 0xE0) == 0x80) {
    *return_value = *offset & 0x0F;
    return true;
  }
  return false;
}

// Lookup a domain key in a byte array generated by make_dafsa.py.
// The rule type is returned if key is found, otherwise kKeyNotFound is
// returned.
static int LookupString(const unsigned char* graph, size_t length,
                        const char* key, size_t key_length) {
  const unsigned char* pos = graph;
  const unsigned char* end = graph + length;
  const unsigned char* offset = pos;
  const char* key_end = key + key_length;
  while (GetNextOffset(&pos, end, &offset)) {
    //   char <char>+ end_char offsets
    //   char <char>+ return value
    //   char end_char offsets
    //   char return value
    //   end_char offsets
    //   return_value
    bool did_consume = false;
    if (key != key_end && !IsEOL(offset, end)) {
      // Leading <char> is not a match. Don't dive into this child
      if (!IsMatch(offset, end, key)) continue;
      did_consume = true;
      ++offset;
      ++key;
      // Possible matches at this point:
      // <char>+ end_char offsets
      // <char>+ return value
      // end_char offsets
      // return value
      // Remove all remaining <char> nodes possible
      while (!IsEOL(offset, end) && key != key_end) {
        if (!IsMatch(offset, end, key)) return mozilla::Dafsa::kKeyNotFound;
        ++key;
        ++offset;
      }
    }
    // Possible matches at this point:
    // end_char offsets
    // return_value
    // If one or more <char> elements were consumed, a failure
    // to match is terminal. Otherwise, try the next node.
    if (key == key_end) {
      int return_value;
      if (GetReturnValue(offset, end, &return_value)) return return_value;
      // The DAFSA guarantees that if the first char is a match, all
      // remaining char elements MUST match if the key is truly present.
      if (did_consume) return mozilla::Dafsa::kKeyNotFound;
      continue;
    }
    if (!IsEndCharMatch(offset, end, key)) {
      if (did_consume) return mozilla::Dafsa::kKeyNotFound;  // Unexpected
      continue;
    }
    ++key;
    pos = ++offset;  // Dive into child
  }
  return mozilla::Dafsa::kKeyNotFound;  // No match
}

namespace mozilla {

int Dafsa::Lookup(const nsACString& aKey) const {
  return LookupString(mData.Elements(), mData.Length(), aKey.BeginReading(),
                      aKey.Length());
}

}  // namespace mozilla
