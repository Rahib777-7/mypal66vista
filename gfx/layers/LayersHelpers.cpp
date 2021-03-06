/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "LayersHelpers.h"

namespace mozilla {
namespace layers {

using namespace gfx;

gfx::IntRect ComputeBackdropCopyRect(const gfx::Rect& aRect,
                                     const gfx::IntRect& aClipRect,
                                     const gfx::Matrix4x4& aTransform,
                                     const gfx::IntRect& aRenderTargetRect,
                                     gfx::Matrix4x4* aOutTransform,
                                     gfx::Rect* aOutLayerQuad) {
  // Compute the clip.
  IntPoint rtOffset = aRenderTargetRect.TopLeft();
  IntSize rtSize = aRenderTargetRect.Size();

  gfx::IntRect renderBounds(0, 0, rtSize.width, rtSize.height);
  renderBounds.IntersectRect(renderBounds, aClipRect);
  renderBounds.MoveBy(rtOffset);

  // Apply the layer transform.
  RectDouble dest = aTransform.TransformAndClipBounds(
      RectDouble(aRect.X(), aRect.Y(), aRect.Width(), aRect.Height()),
      RectDouble(renderBounds.X(), renderBounds.Y(), renderBounds.Width(),
                 renderBounds.Height()));
  dest -= rtOffset;

  // Ensure we don't round out to -1, which trips up Direct3D.
  dest.IntersectRect(dest, RectDouble(0, 0, rtSize.width, rtSize.height));

  if (aOutLayerQuad) {
    *aOutLayerQuad = Rect(dest.X(), dest.Y(), dest.Width(), dest.Height());
  }

  // Round out to integer.
  IntRect result;
  dest.RoundOut();
  dest.ToIntRect(&result);

  // Create a transform from adjusted clip space to render target space,
  // translate it for the backdrop rect, then transform it into the backdrop's
  // uv-space.
  Matrix4x4 transform;
  transform.PostScale(rtSize.width, rtSize.height, 1.0);
  transform.PostTranslate(-result.X(), -result.Y(), 0.0);
  transform.PostScale(1 / float(result.Width()), 1 / float(result.Height()),
                      1.0);
  *aOutTransform = transform;
  return result;
}

}  // namespace layers
}  // namespace mozilla
