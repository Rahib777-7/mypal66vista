/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef COMPONENTS_PERFMEASUREMENT_H
#define COMPONENTS_PERFMEASUREMENT_H

#include "nsIXPCScriptable.h"
#include "mozilla/Attributes.h"

namespace mozilla {
namespace jsperf {

class Module final : public nsIXPCScriptable {
 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCSCRIPTABLE

  Module();

 private:
  ~Module();
};

}  // namespace jsperf
}  // namespace mozilla

#endif
