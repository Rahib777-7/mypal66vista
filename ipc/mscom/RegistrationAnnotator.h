/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_mscom_RegistrationAnnotator_h
#define mozilla_mscom_RegistrationAnnotator_h

namespace mozilla {
namespace mscom {

void AnnotateInterfaceRegistration(REFIID aIid);
void AnnotateClassRegistration(REFCLSID aClsid);

}  // namespace mscom
}  // namespace mozilla

#endif  // mozilla_mscom_RegistrationAnnotator_h
