/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "PendingIPCBlobParent.h"
#include "mozilla/ipc/PBackgroundParent.h"

namespace mozilla {

using namespace ipc;

namespace dom {

/* static */
PendingIPCBlobParent* PendingIPCBlobParent::Create(PBackgroundParent* aManager,
                                                   BlobImpl* aBlobImpl) {
  MOZ_ASSERT(aManager);
  MOZ_ASSERT(aBlobImpl);

  IPCBlob ipcBlob;
  nsresult rv = IPCBlobUtils::Serialize(aBlobImpl, aManager, ipcBlob);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return nullptr;
  }

  PendingIPCBlobParent* actor = new PendingIPCBlobParent(aBlobImpl);
  if (!aManager->SendPPendingIPCBlobConstructor(actor, ipcBlob)) {
    // The actor will be deleted by the manager.
    return nullptr;
  }

  return actor;
}

PendingIPCBlobParent::PendingIPCBlobParent(BlobImpl* aBlobImpl)
    : mBlobImpl(aBlobImpl) {}

IPCResult PendingIPCBlobParent::Recv__delete__(
    const PendingIPCBlobData& aData) {
  if (aData.file().isNothing()) {
    mBlobImpl->SetLazyData(VoidString(), aData.type(), aData.size(), INT64_MAX);
  } else {
    const PendingIPCFileData& fileData = aData.file().ref();
    mBlobImpl->SetLazyData(fileData.name(), aData.type(), aData.size(),
                           fileData.lastModified());
  }

  return IPC_OK();
}

}  // namespace dom
}  // namespace mozilla
