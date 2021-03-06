/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_quota_ActorsParent_h
#define mozilla_dom_quota_ActorsParent_h

namespace mozilla {
namespace dom {
namespace quota {

class PQuotaParent;

void InitializeQuotaManager();

PQuotaParent* AllocPQuotaParent();

bool DeallocPQuotaParent(PQuotaParent* aActor);

bool RecvShutdownQuotaManager();

}  // namespace quota
}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_quota_ActorsParent_h
