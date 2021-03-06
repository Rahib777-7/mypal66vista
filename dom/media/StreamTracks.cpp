/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "StreamTracks.h"
#include "mozilla/Logging.h"
#include <algorithm>

namespace mozilla {

#ifdef DEBUG

extern LazyLogModule gMediaStreamGraphLog;
#  define STREAM_LOG(type, msg) MOZ_LOG(gMediaStreamGraphLog, type, msg)

void StreamTracks::DumpTrackInfo() const {
  STREAM_LOG(LogLevel::Info, ("Dumping StreamTracks"));
  for (uint32_t i = 0; i < mTracks.Length(); ++i) {
    Track* track = mTracks[i];
    if (track->IsEnded()) {
      STREAM_LOG(LogLevel::Info, ("Track[%d] %d: ended", i, track->GetID()));
    } else {
      STREAM_LOG(LogLevel::Info, ("Track[%d] %d: %" PRId64 "", i,
                                  track->GetID(), track->GetEnd()));
    }
  }
}
#endif

StreamTime StreamTracks::GetEarliestTrackEnd() const {
  StreamTime t = STREAM_TIME_MAX;
  for (uint32_t i = 0; i < mTracks.Length(); ++i) {
    Track* track = mTracks[i];
    if (!track->IsEnded()) {
      t = std::min(t, track->GetEnd());
    }
  }
  return t;
}

StreamTime StreamTracks::GetLatestTrackEnd() const {
  StreamTime t = 0;
  for (uint32_t i = 0; i < mTracks.Length(); ++i) {
    Track* track = mTracks[i];
    if (!track->IsEnded()) {
      return STREAM_TIME_MAX;
    }
    t = std::max(t, track->GetEnd());
  }
  return t;
}

StreamTracks::Track* StreamTracks::FindTrack(TrackID aID) const {
  if (aID == TRACK_NONE || mTracks.IsEmpty()) {
    return nullptr;
  }

  // The tracks are sorted by ID. We can use a binary search.

  uint32_t left = 0, right = mTracks.Length() - 1;
  while (left <= right) {
    uint32_t middle = (left + right) / 2;
    if (mTracks[middle]->GetID() == aID) {
      return mTracks[middle];
    }

    if (mTracks[middle]->GetID() > aID) {
      if (middle == 0) {
        break;
      }

      right = middle - 1;
    } else {
      left = middle + 1;
    }
  }

  return nullptr;
}

void StreamTracks::ForgetUpTo(StreamTime aTime) {
  // Only prune if there is a reasonable chunk (50ms @ 48kHz) to forget, so we
  // don't spend too much time pruning segments.
  const StreamTime minChunkSize = 2400;
  if (aTime < mForgottenTime + minChunkSize) {
    return;
  }
  mForgottenTime = aTime;

  for (uint32_t i = 0; i < mTracks.Length(); ++i) {
    Track* track = mTracks[i];
    StreamTime forgetTo = std::min(track->GetEnd() - 1, aTime);
    track->ForgetUpTo(forgetTo);
  }
}

void StreamTracks::Clear() { mTracks.Clear(); }

}  // namespace mozilla
