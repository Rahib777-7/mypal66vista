/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko.activitystream.homepanel.model;

import android.database.Cursor;
import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.util.Log;
import org.json.JSONException;
import org.json.JSONObject;

public class Metadata {
    private static final String LOGTAG = "GeckoMetadata";

    public static Metadata fromCursor(final Cursor cursor, final String columnName) {
        return new Metadata(
                cursor.getString(cursor.getColumnIndexOrThrow(columnName)));
    }

    private String provider;
    private String imageUrl;
    private int descriptionLength;

    /* package-private */ Metadata(String json) {
        if (TextUtils.isEmpty(json)) {
            // Just use default values. It's better to have an empty Metadata object instead of
            // juggling with null values.
            return;
        }

        try {
            JSONObject object = new JSONObject(json);

            provider = object.optString("provider");
            imageUrl = object.optString("image_url");
            descriptionLength = object.optInt("description_length");
        } catch (JSONException e) {
            Log.w(LOGTAG, "JSONException while parsing metadata", e);
        }
    }

    public boolean hasProvider() {
        return !TextUtils.isEmpty(provider);
    }

    /**
     * Returns the URL of an image representing this site. Returns null if no image could be found.
     */
    public @Nullable String getImageUrl() {
        return imageUrl;
    }

    public String getProvider() {
        return provider;
    }

    public int getDescriptionLength() {
        return descriptionLength;
    }
}
