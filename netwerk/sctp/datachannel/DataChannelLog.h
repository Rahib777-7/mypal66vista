/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DataChannelLog_h
#define DataChannelLog_h

#include "base/basictypes.h"
#include "mozilla/Logging.h"

namespace mozilla {
extern mozilla::LazyLogModule gDataChannelLog;
}

#undef LOG
#define LOG(args) \
  MOZ_LOG(mozilla::gDataChannelLog, mozilla::LogLevel::Debug, args)

#endif
