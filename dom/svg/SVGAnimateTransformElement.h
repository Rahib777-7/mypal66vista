/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_SVGAnimateTransformElement_h
#define mozilla_dom_SVGAnimateTransformElement_h

#include "mozilla/Attributes.h"
#include "mozilla/dom/SVGAnimationElement.h"
#include "mozilla/SMILAnimationFunction.h"

nsresult NS_NewSVGAnimateTransformElement(
    nsIContent** aResult, already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo);

namespace mozilla {
namespace dom {

class SVGAnimateTransformElement final : public SVGAnimationElement {
 protected:
  explicit SVGAnimateTransformElement(
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo);

  SMILAnimationFunction mAnimationFunction;
  friend nsresult(::NS_NewSVGAnimateTransformElement(
      nsIContent** aResult,
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo));

  virtual JSObject* WrapNode(JSContext* aCx,
                             JS::Handle<JSObject*> aGivenProto) override;

 public:
  // nsINode specializations
  virtual nsresult Clone(dom::NodeInfo*, nsINode** aResult) const override;

  // Element specializations
  virtual bool ParseAttribute(int32_t aNamespaceID, nsAtom* aAttribute,
                              const nsAString& aValue,
                              nsIPrincipal* aMaybeScriptedPrincipal,
                              nsAttrValue& aResult) override;

  // SVGAnimationElement
  virtual SMILAnimationFunction& AnimationFunction() override;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_SVGAnimateTransformElement_h
