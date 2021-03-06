/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* For documentation of the accessibility architecture,
 * see http://lxr.mozilla.org/seamonkey/source/accessible/accessible-docs.html
 */

#ifndef mozilla_a11y_DocAccessibleWrap_h__
#define mozilla_a11y_DocAccessibleWrap_h__

#include "DocAccessible.h"

namespace mozilla {

class PresShell;

namespace a11y {

class DocAccessibleWrap : public DocAccessible {
 public:
  DocAccessibleWrap(dom::Document* aDocument, PresShell* aPresShell);
  virtual ~DocAccessibleWrap();

  bool mActivated;
};

}  // namespace a11y
}  // namespace mozilla

#endif
