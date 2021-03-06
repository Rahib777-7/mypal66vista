/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko.activitystream.homepanel.model;

import org.mozilla.gecko.activitystream.Utils;

/**
 * Model for a row in Activity Stream that represents a webpage item.
 */
public interface WebpageRowModel extends WebpageModel, RowModel {
    Utils.HighlightSource getSource();
    long getUniqueId();
}
