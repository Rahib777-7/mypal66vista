/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef StorageAccessPermissionRequest_h_
#define StorageAccessPermissionRequest_h_

#include "nsContentPermissionHelper.h"

#include <functional>

class nsPIDOMWindowInner;

namespace mozilla {
namespace dom {

class StorageAccessPermissionRequest final
    : public ContentPermissionRequestBase {
 public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(StorageAccessPermissionRequest,
                                           ContentPermissionRequestBase)

  // nsIContentPermissionRequest
  NS_IMETHOD Cancel(void) override;
  NS_IMETHOD Allow(JS::HandleValue choices) override;

  typedef std::function<void()> AllowCallback;
  typedef std::function<void()> AllowAutoGrantCallback;
  typedef std::function<void()> AllowAnySiteCallback;
  typedef std::function<void()> CancelCallback;

  static already_AddRefed<StorageAccessPermissionRequest> Create(
      nsPIDOMWindowInner* aWindow, AllowCallback&& aAllowCallback,
      AllowAutoGrantCallback&& aAllowAutoGrantCallback,
      AllowAnySiteCallback&& aAllowAnySiteCallback,
      CancelCallback&& aCancelCallback);

 private:
  StorageAccessPermissionRequest(
      nsPIDOMWindowInner* aWindow, nsIPrincipal* aNodePrincipal,
      AllowCallback&& aAllowCallback,
      AllowAutoGrantCallback&& aAllowAutoGrantCallback,
      AllowAnySiteCallback&& aAllowAnySiteCallback,
      CancelCallback&& aCancelCallback);
  ~StorageAccessPermissionRequest();

  unsigned CalculateSimulatedDelay();

  static void CallAutoGrantCallback(nsITimer* aTimer, void* aClosure);

  AllowCallback mAllowCallback;
  AllowAutoGrantCallback mAllowAutoGrantCallback;
  AllowAnySiteCallback mAllowAnySiteCallback;
  CancelCallback mCancelCallback;
  nsCOMPtr<nsITimer> mTimer;
  nsTArray<PermissionRequest> mPermissionRequests;
  bool mCallbackCalled;
};

}  // namespace dom
}  // namespace mozilla

#endif  // StorageAccessPermissionRequest_h_
