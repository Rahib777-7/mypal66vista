/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SocketProcessBridgeParent.h"
#include "SocketProcessLogging.h"

#include "mozilla/ipc/BackgroundParent.h"
#include "SocketProcessChild.h"
#include "mozilla/dom/MediaTransportParent.h"

namespace mozilla {
namespace net {

SocketProcessBridgeParent::SocketProcessBridgeParent(
    ProcessId aId, Endpoint<PSocketProcessBridgeParent>&& aEndpoint)
    : mId(aId) {
  LOG((
      "CONSTRUCT SocketProcessBridgeParent::SocketProcessBridgeParent mId=%d\n",
      mId));
  MOZ_COUNT_CTOR(SocketProcessBridgeParent);
  DebugOnly<bool> ok = aEndpoint.Bind(this);
  MOZ_ASSERT(ok);
}

SocketProcessBridgeParent::~SocketProcessBridgeParent() {
  LOG(("DESTRUCT SocketProcessBridgeParent::SocketProcessBridgeParent\n"));
  MOZ_COUNT_DTOR(SocketProcessBridgeParent);
}

mozilla::ipc::IPCResult SocketProcessBridgeParent::RecvTest() {
  LOG(("SocketProcessBridgeParent::RecvTest\n"));
  Unused << SendTest();
  return IPC_OK();
}

mozilla::ipc::IPCResult SocketProcessBridgeParent::RecvInitBackground(
    Endpoint<PBackgroundParent>&& aEndpoint) {
  LOG(("SocketProcessBridgeParent::RecvInitBackground mId=%d\n", mId));
  if (!ipc::BackgroundParent::Alloc(nullptr, std::move(aEndpoint))) {
    return IPC_FAIL(this, "BackgroundParent::Alloc failed");
  }

  return IPC_OK();
}

void SocketProcessBridgeParent::ActorDestroy(ActorDestroyReason aWhy) {
  LOG(("SocketProcessBridgeParent::ActorDestroy mId=%d\n", mId));

  MessageLoop::current()->PostTask(
      NewRunnableMethod("net::SocketProcessBridgeParent::DeferredDestroy", this,
                        &SocketProcessBridgeParent::DeferredDestroy));
}

void SocketProcessBridgeParent::DeferredDestroy() {
  SocketProcessChild::GetSingleton()->DestroySocketProcessBridgeParent(mId);
}

dom::PMediaTransportParent*
SocketProcessBridgeParent::AllocPMediaTransportParent() {
#ifdef MOZ_WEBRTC
  return new MediaTransportParent;
#endif
  return nullptr;
}

bool SocketProcessBridgeParent::DeallocPMediaTransportParent(
    dom::PMediaTransportParent* aActor) {
#ifdef MOZ_WEBRTC
  delete aActor;
#endif
  return true;
}

}  // namespace net
}  // namespace mozilla
