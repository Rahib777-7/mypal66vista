/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_presentation_DeviceProviderHelpers_h
#define mozilla_dom_presentation_DeviceProviderHelpers_h

#include "nsString.h"

namespace mozilla {
namespace dom {
namespace presentation {

class DeviceProviderHelpers final {
 public:
  static bool IsCommonlySupportedScheme(const nsAString& aUrl);
  static bool IsFxTVSupportedAppUrl(const nsAString& aUrl);

 private:
  DeviceProviderHelpers() = delete;
};

}  // namespace presentation
}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_presentation_DeviceProviderHelpers_h
