/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_RENDERROOTTYPES_H
#define GFX_RENDERROOTTYPES_H

#include "mozilla/webrender/WebRenderAPI.h"
#include "mozilla/webrender/WebRenderTypes.h"
#include "mozilla/layers/WebRenderMessages.h"
#include "mozilla/layers/WebRenderScrollData.h"

namespace mozilla {

namespace layers {

struct RenderRootDisplayListData {
  wr::RenderRoot mRenderRoot;
  LayoutDeviceRect mRect;
  nsTArray<WebRenderParentCommand> mCommands;
  wr::LayoutSize mContentSize;
  Maybe<mozilla::ipc::ByteBuf> mDL;
  wr::BuiltDisplayListDescriptor mDLDesc;
  nsTArray<OpUpdateResource> mResourceUpdates;
  nsTArray<RefCountedShmem> mSmallShmems;
  nsTArray<mozilla::ipc::Shmem> mLargeShmems;
  Maybe<WebRenderScrollData> mScrollData;
};

struct RenderRootUpdates {
  wr::RenderRoot mRenderRoot;
  nsTArray<WebRenderParentCommand> mCommands;
  nsTArray<OpUpdateResource> mResourceUpdates;
  nsTArray<RefCountedShmem> mSmallShmems;
  nsTArray<mozilla::ipc::Shmem> mLargeShmems;
  ScrollUpdatesMap mScrollUpdates;
};

}  // namespace layers

namespace ipc {

template <>
struct IPDLParamTraits<mozilla::layers::RenderRootDisplayListData> {
  typedef mozilla::layers::RenderRootDisplayListData paramType;

  static void Write(IPC::Message* aMsg, IProtocol* aActor, paramType&& aParam);

  static bool Read(const IPC::Message* aMsg, PickleIterator* aIter,
                   IProtocol* aActor, paramType* aResult);
};

template <>
struct IPDLParamTraits<mozilla::layers::RenderRootUpdates> {
  typedef mozilla::layers::RenderRootUpdates paramType;

  static void Write(IPC::Message* aMsg, IProtocol* aActor, paramType&& aParam);

  static bool Read(const IPC::Message* aMsg, PickleIterator* aIter,
                   IProtocol* aActor, paramType* aResult);
};

}  // namespace ipc
}  // namespace mozilla

#endif /* GFX_RENDERROOTTYPES_H */
