/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef _mozilla_dom_ClientOpenWindowOpActors_h
#define _mozilla_dom_ClientOpenWindowOpActors_h

namespace mozilla {
namespace dom {

class ClientOpenWindowArgs;
class PClientOpenWindowOpChild;
class PClientOpenWindowOpParent;

PClientOpenWindowOpChild* AllocClientOpenWindowOpChild();

void InitClientOpenWindowOpChild(PClientOpenWindowOpChild* aActor,
                                 const ClientOpenWindowArgs& aArgs);

bool DeallocClientOpenWindowOpChild(PClientOpenWindowOpChild* aActor);

PClientOpenWindowOpParent* AllocClientOpenWindowOpParent(
    const ClientOpenWindowArgs& aArgs);

bool DeallocClientOpenWindowOpParent(PClientOpenWindowOpParent* aActor);

}  // namespace dom
}  // namespace mozilla

#endif  // _mozilla_dom_ClientOpenWindowOpActors_h
