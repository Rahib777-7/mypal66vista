/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsImportModule.h"

#include "mozilla/ResultExtensions.h"
#include "mozilla/dom/ScriptSettings.h"
#include "mozJSComponentLoader.h"
#include "xpcpublic.h"
#include "xpcprivate.h"

using mozilla::dom::AutoJSAPI;

namespace mozilla {
namespace loader {

nsresult ImportModule(const char* aURI, const nsIID& aIID, void** aResult) {
  AutoJSAPI jsapi;
  MOZ_ALWAYS_TRUE(jsapi.Init(xpc::PrivilegedJunkScope()));
  JSContext* cx = jsapi.cx();

  JS::RootedObject global(cx);
  JS::RootedObject exports(cx);
  MOZ_TRY(mozJSComponentLoader::Get()->Import(cx, nsDependentCString(aURI),
                                              &global, &exports));

  return nsXPConnect::XPConnect()->WrapJS(cx, exports, aIID, aResult);
}

}  // namespace loader
}  // namespace mozilla
