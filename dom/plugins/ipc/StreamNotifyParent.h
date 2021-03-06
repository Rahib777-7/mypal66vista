/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_plugins_StreamNotifyParent_h
#define mozilla_plugins_StreamNotifyParent_h

#include "mozilla/plugins/PStreamNotifyParent.h"

namespace mozilla {
namespace plugins {

class StreamNotifyParent : public PStreamNotifyParent {
  friend class PluginInstanceParent;
  friend class PStreamNotifyParent;

  StreamNotifyParent() : mDestructionFlag(nullptr) {}
  ~StreamNotifyParent() {
    if (mDestructionFlag) *mDestructionFlag = true;
  }

 public:
  // If we are destroyed within the call to NPN_GetURLNotify, notify the caller
  // so that we aren't destroyed again. see bug 536437.
  void SetDestructionFlag(bool* flag) { mDestructionFlag = flag; }
  void ClearDestructionFlag() { mDestructionFlag = nullptr; }

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

 private:
  mozilla::ipc::IPCResult RecvRedirectNotifyResponse(const bool& allow);

  bool* mDestructionFlag;
};

}  // namespace plugins
}  // namespace mozilla

#endif
