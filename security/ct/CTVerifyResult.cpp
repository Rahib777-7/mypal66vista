/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CTVerifyResult.h"

#include <stdint.h>

namespace mozilla {
namespace ct {

VerifiedSCT::VerifiedSCT()
    : status(Status::None),
      origin(Origin::Unknown),
      logOperatorId(-1),
      logDisqualificationTime(UINT64_MAX) {}

void CTVerifyResult::Reset() {
  verifiedScts.clear();
  decodingErrors = 0;
}

}  // namespace ct
}  // namespace mozilla
