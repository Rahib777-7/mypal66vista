/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_MIDIMessageQueue_h
#define mozilla_dom_MIDIMessageQueue_h

#include "nsTArray.h"

namespace mozilla {
namespace dom {

class MIDIMessage;

/**
 * Since some MIDI Messages can be scheduled to be sent in the future, the
 * MIDIMessageQueue is responsible for making sure all MIDI messages are
 * scheduled and sent in order.
 */
class MIDIMessageQueue {
 public:
  MIDIMessageQueue();
  ~MIDIMessageQueue() = default;
  // Adds an array of possibly out-of-order messages to our queue.
  void Add(nsTArray<MIDIMessage>& aMsg);
  // Retrieve all pending messages before the time specified.
  void GetMessagesBefore(TimeStamp aTimestamp,
                         nsTArray<MIDIMessage>& aMsgArray);
  // Get all pending messages.
  void GetMessages(nsTArray<MIDIMessage>& aMsgArray);
  // Erase all pending messages.
  void Clear();
  // Erase all pending messages that would be sent in the future. Useful for
  // when ports are closed, as we must send all messages with timestamps in the
  // past.
  void ClearAfterNow();

 private:
  // Array of messages to be sent.
  nsTArray<MIDIMessage> mMessageQueue;
  // Mutex for coordinating cross thread array access.
  Mutex mMutex;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_MIDIMessageQueue_h
