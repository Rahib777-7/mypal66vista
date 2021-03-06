/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_PresentationLog_h
#define mozilla_dom_PresentationLog_h

/*
 * MOZ_LOG=Presentation:5
 * For detail, see PresentationService.cpp
 */
namespace mozilla {
namespace dom {
extern mozilla::LazyLogModule gPresentationLog;
}
}  // namespace mozilla

#undef PRES_ERROR
#define PRES_ERROR(...)                                             \
  MOZ_LOG(mozilla::dom::gPresentationLog, mozilla::LogLevel::Error, \
          (__VA_ARGS__))

#undef PRES_DEBUG
#define PRES_DEBUG(...)                                             \
  MOZ_LOG(mozilla::dom::gPresentationLog, mozilla::LogLevel::Debug, \
          (__VA_ARGS__))

#endif  // mozilla_dom_PresentationLog_h
