/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_WindowGlobalActor_h
#define mozilla_dom_WindowGlobalActor_h

#include "nsWrapperCache.h"
#include "nsISupports.h"
#include "mozilla/dom/BrowsingContext.h"
#include "mozilla/ErrorResult.h"
#include "nsIURI.h"
#include "nsString.h"
#include "mozilla/dom/JSWindowActor.h"

namespace mozilla {
namespace dom {

// Common base class for WindowGlobal{Parent, Child}.
class WindowGlobalActor : public nsISupports, public nsWrapperCache {
 public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(WindowGlobalActor)

 protected:
  virtual ~WindowGlobalActor() = default;

  // Load the module for the named Window Actor and contruct it.
  // This method will not initialize the actor or set its manager,
  // which is handled by callers.
  void ConstructActor(const nsAString& aName, JS::MutableHandleObject aActor,
                      ErrorResult& aRv);
  virtual nsIURI* GetDocumentURI() = 0;
  virtual const nsAString& GetRemoteType() = 0;
  virtual JSWindowActor::Type GetSide() = 0;
  virtual dom::BrowsingContext* BrowsingContext() = 0;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_WindowGlobalActor_h