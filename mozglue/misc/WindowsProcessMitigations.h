/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_WindowsProcessMitigations_h
#define mozilla_WindowsProcessMitigations_h

#include "mozilla/Types.h"

namespace mozilla {

MFBT_API bool IsWin32kLockedDown();
MFBT_API bool IsDynamicCodeDisabled();

}  // namespace mozilla

#endif  // mozilla_WindowsProcessMitigations_h
