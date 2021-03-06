/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ServiceWorkerUpdaterParent.h"
#include "ServiceWorkerManagerService.h"

namespace mozilla {
namespace dom {

bool ServiceWorkerUpdaterParent::Proceed(
    ServiceWorkerManagerService* aService) {
  if (!SendProceed(true)) {
    return false;
  }

  mService = aService;
  return true;
}

void ServiceWorkerUpdaterParent::ActorDestroy(ActorDestroyReason aWhy) {
  if (mService) {
    mService->UpdaterActorDestroyed(this);
  }
}

}  // namespace dom
}  // namespace mozilla
