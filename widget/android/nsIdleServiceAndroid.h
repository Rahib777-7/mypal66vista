/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsIdleServiceAndroid_h__
#define nsIdleServiceAndroid_h__

#include "nsIdleService.h"

class nsIdleServiceAndroid : public nsIdleService {
 public:
  NS_INLINE_DECL_REFCOUNTING_INHERITED(nsIdleServiceAndroid, nsIdleService)

  bool PollIdleTime(uint32_t* aIdleTime) override;

  static already_AddRefed<nsIdleServiceAndroid> GetInstance() {
    RefPtr<nsIdleService> idleService = nsIdleService::GetInstance();
    if (!idleService) {
      idleService = new nsIdleServiceAndroid();
    }

    return idleService.forget().downcast<nsIdleServiceAndroid>();
  }

 protected:
  nsIdleServiceAndroid() {}
  virtual ~nsIdleServiceAndroid() {}
  bool UsePollMode() override;
};

#endif  // nsIdleServiceAndroid_h__
