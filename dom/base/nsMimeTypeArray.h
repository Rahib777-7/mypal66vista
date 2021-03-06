/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsMimeTypeArray_h___
#define nsMimeTypeArray_h___

#include "nsString.h"
#include "nsTArray.h"
#include "nsWrapperCache.h"
#include "nsPIDOMWindow.h"
#include "mozilla/dom/BindingDeclarations.h"

class nsMimeType;
class nsPluginElement;

class nsMimeTypeArray final : public nsISupports, public nsWrapperCache {
 public:
  explicit nsMimeTypeArray(nsPIDOMWindowInner* aWindow);

  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(nsMimeTypeArray)

  nsPIDOMWindowInner* GetParentObject() const;
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;

  void Refresh();

  // MimeTypeArray WebIDL methods
  nsMimeType* Item(uint32_t index, mozilla::dom::CallerType aCallerType);
  nsMimeType* NamedItem(const nsAString& name,
                        mozilla::dom::CallerType aCallerType);
  nsMimeType* IndexedGetter(uint32_t index, bool& found,
                            mozilla::dom::CallerType aCallerType);
  nsMimeType* NamedGetter(const nsAString& name, bool& found,
                          mozilla::dom::CallerType aCallerType);
  uint32_t Length(mozilla::dom::CallerType aCallerType);
  void GetSupportedNames(nsTArray<nsString>& retval,
                         mozilla::dom::CallerType aCallerType);

 protected:
  virtual ~nsMimeTypeArray();

  void EnsurePluginMimeTypes();
  void Clear();

  nsCOMPtr<nsPIDOMWindowInner> mWindow;

  // mMimeTypes contains MIME types handled by plugins or by an OS
  // PreferredApplicationHandler.
  nsTArray<RefPtr<nsMimeType> > mMimeTypes;
  nsTArray<RefPtr<nsMimeType> > mCTPMimeTypes;
};

class nsMimeType final : public nsWrapperCache {
 public:
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(nsMimeType)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(nsMimeType)

  nsMimeType(nsPIDOMWindowInner* aWindow, nsPluginElement* aPluginElement,
             const nsAString& aType, const nsAString& aDescription,
             const nsAString& aExtension);
  nsPIDOMWindowInner* GetParentObject() const;
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;

  const nsString& Type() const { return mType; }

  // MimeType WebIDL methods
  void GetDescription(nsString& retval) const;
  nsPluginElement* GetEnabledPlugin() const;
  void GetSuffixes(nsString& retval) const;
  void GetType(nsString& retval) const;

 protected:
  virtual ~nsMimeType();

  nsCOMPtr<nsPIDOMWindowInner> mWindow;

  // Strong reference to the active plugin. Note that this creates an explicit
  // reference cycle through the plugin element's mimetype array. We rely on the
  // cycle collector to break this cycle.
  RefPtr<nsPluginElement> mPluginElement;
  nsString mType;
  nsString mDescription;
  nsString mExtension;
};

#endif /* nsMimeTypeArray_h___ */
