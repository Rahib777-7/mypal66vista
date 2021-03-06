/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef FrameChildList_h_
#define FrameChildList_h_

#include "mozilla/EnumSet.h"
#include "nsFrameList.h"
#include "nsTArray.h"

class nsIFrame;

namespace mozilla {
namespace layout {

// enum FrameChildListID lives in nsFrameList.h to solve circular dependencies.

#ifdef DEBUG_FRAME_DUMP
extern const char* ChildListName(FrameChildListID aListID);
#endif

using FrameChildListIDs = EnumSet<FrameChildListID>;

class FrameChildList {
 public:
  FrameChildList(const nsFrameList& aList, FrameChildListID aID)
      : mList(aList), mID(aID) {}
  nsFrameList mList;
  FrameChildListID mID;
};

/**
 * A class to iterate frame child lists.
 */
class MOZ_STACK_CLASS FrameChildListArrayIterator {
 public:
  explicit FrameChildListArrayIterator(const nsTArray<FrameChildList>& aLists)
      : mLists(aLists), mCurrentIndex(0) {}
  bool IsDone() const { return mCurrentIndex >= mLists.Length(); }
  FrameChildListID CurrentID() const {
    NS_ASSERTION(!IsDone(), "CurrentID(): iterator at end");
    return mLists[mCurrentIndex].mID;
  }
  const nsFrameList& CurrentList() const {
    NS_ASSERTION(!IsDone(), "CurrentList(): iterator at end");
    return mLists[mCurrentIndex].mList;
  }
  void Next() {
    NS_ASSERTION(!IsDone(), "Next(): iterator at end");
    ++mCurrentIndex;
  }

 protected:
  const nsTArray<FrameChildList>& mLists;
  uint32_t mCurrentIndex;
};

/**
 * A class for retrieving a frame's child lists and iterate them.
 */
class MOZ_STACK_CLASS FrameChildListIterator
    : public FrameChildListArrayIterator {
 public:
  explicit FrameChildListIterator(const nsIFrame* aFrame);

 protected:
  AutoTArray<FrameChildList, 4> mLists;
};

}  // namespace layout
}  // namespace mozilla

inline void nsFrameList::AppendIfNonempty(
    nsTArray<mozilla::layout::FrameChildList>* aLists,
    mozilla::layout::FrameChildListID aListID) const {
  if (NotEmpty()) {
    aLists->AppendElement(mozilla::layout::FrameChildList(*this, aListID));
  }
}

#endif /* !defined(FrameChildList_h_) */
