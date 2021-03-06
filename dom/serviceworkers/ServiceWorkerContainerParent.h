/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_serviceworkercontainerparent_h__
#define mozilla_dom_serviceworkercontainerparent_h__

#include "mozilla/dom/PServiceWorkerContainerParent.h"

namespace mozilla {
namespace dom {

class IPCServiceWorkerDescriptor;
class ServiceWorkerContainerProxy;

class ServiceWorkerContainerParent final
    : public PServiceWorkerContainerParent {
  RefPtr<ServiceWorkerContainerProxy> mProxy;

  // PServiceWorkerContainerParent
  void ActorDestroy(ActorDestroyReason aReason) override;

  mozilla::ipc::IPCResult RecvTeardown() override;

  mozilla::ipc::IPCResult RecvRegister(
      const IPCClientInfo& aClientInfo, const nsCString& aScopeURL,
      const nsCString& aScriptURL,
      const ServiceWorkerUpdateViaCache& aUpdateViaCache,
      RegisterResolver&& aResolver) override;

  mozilla::ipc::IPCResult RecvGetRegistration(
      const IPCClientInfo& aClientInfo, const nsCString& aURL,
      GetRegistrationResolver&& aResolver) override;

  mozilla::ipc::IPCResult RecvGetRegistrations(
      const IPCClientInfo& aClientInfo,
      GetRegistrationsResolver&& aResolver) override;

  mozilla::ipc::IPCResult RecvGetReady(const IPCClientInfo& aClientInfo,
                                       GetReadyResolver&& aResolver) override;

 public:
  ServiceWorkerContainerParent();
  ~ServiceWorkerContainerParent();

  void Init();
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_serviceworkercontainerparent_h__
