/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko;

import org.mozilla.gecko.annotation.WrapForJNI;

class ScreenManagerHelper {

    /**
     * The following display types use the same definition in nsIScreen.idl
     */
    final static int DISPLAY_PRIMARY  = 0; // primary screen
    final static int DISPLAY_EXTERNAL = 1; // wired displays, such as HDMI, DisplayPort, etc.
    final static int DISPLAY_VIRTUAL  = 2; // wireless displays, such as Chromecast, WiFi-Display, etc.

    /**
     * Trigger a refresh of the cached screen information held by Gecko.
     */
    public static void refreshScreenInfo() {
        // Screen data is initialised automatically on startup, so no need to queue the call if
        // Gecko isn't running yet.
        if (GeckoThread.isRunning()) {
            nativeRefreshScreenInfo();
        }
    }

    @WrapForJNI(stubName = "RefreshScreenInfo", dispatchTo = "gecko")
    private native static void nativeRefreshScreenInfo();

    /**
     * Add a new nsScreen when a new display in Android is available.
     *
     * @param displayType the display type of the nsScreen would be added
     * @param width       the width of the new nsScreen
     * @param height      the height of the new nsScreen
     * @param density     the density of the new nsScreen
     *
     * @return            return the ID of the added nsScreen
     */
    @WrapForJNI
    public native static int addDisplay(int displayType,
                                        int width,
                                        int height,
                                        float density);

    /**
     * Remove the nsScreen by the specific screen ID.
     *
     * @param screenId    the ID of the screen would be removed.
     */
    @WrapForJNI
    public native static void removeDisplay(int screenId);
}
