/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef FileInputType_h__
#define FileInputType_h__

#include "InputType.h"

// input type=file
class FileInputType : public ::InputType {
 public:
  static InputType* Create(mozilla::dom::HTMLInputElement* aInputElement,
                           void* aMemory) {
    return new (aMemory) FileInputType(aInputElement);
  }

  bool IsValueMissing() const override;

  nsresult GetValueMissingMessage(nsAString& aMessage) override;

 private:
  explicit FileInputType(mozilla::dom::HTMLInputElement* aInputElement)
      : InputType(aInputElement) {}
};

#endif /* FileInputType_h__ */
