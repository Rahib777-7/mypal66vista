/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_layers_CompositorVsyncSchedulerOwner_h
#define mozilla_layers_CompositorVsyncSchedulerOwner_h

#include "mozilla/VsyncDispatcher.h"

namespace mozilla {

namespace gfx {
class DrawTarget;
}  // namespace gfx

namespace layers {

class CompositorVsyncSchedulerOwner {
 public:
  virtual bool IsPendingComposite() = 0;
  virtual void FinishPendingComposite() = 0;
  virtual void CompositeToTarget(VsyncId aId, gfx::DrawTarget* aTarget,
                                 const gfx::IntRect* aRect = nullptr) = 0;
  virtual TimeDuration GetVsyncInterval() const = 0;
};

}  // namespace layers
}  // namespace mozilla

#endif  // mozilla_layers_CompositorVsyncSchedulerOwner_h
