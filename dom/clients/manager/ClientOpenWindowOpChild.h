/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef _mozilla_dom_ClientOpenWindowOpChild_h
#define _mozilla_dom_ClientOpenWindowOpChild_h

#include "mozilla/dom/PClientOpenWindowOpChild.h"
#include "ClientOpPromise.h"

namespace mozilla {
namespace dom {

class ClientOpenWindowOpChild final : public PClientOpenWindowOpChild {
  MozPromiseRequestHolder<ClientOpPromise> mPromiseRequestHolder;

  MOZ_MUST_USE RefPtr<ClientOpPromise> DoOpenWindow(
      const ClientOpenWindowArgs& aArgs);

  // PClientOpenWindowOpChild interface
  void ActorDestroy(ActorDestroyReason aReason) override;

 public:
  ClientOpenWindowOpChild() = default;
  ~ClientOpenWindowOpChild() = default;

  void Init(const ClientOpenWindowArgs& aArgs);
};

}  // namespace dom
}  // namespace mozilla

#endif  // _mozilla_dom_ClientOpenWindowOpChild_h
