/*
 * Copyright (C) 2012-2013 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.cyanogenmod.settings.device;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceManager;

public class WakeMethod implements OnPreferenceChangeListener {

    private static final String FILE_DT2W = "/sys/android_touch/doubletap2wake";
    private static final String FILE_S2W = "/sys/android_touch/sweep2wake";

    private static final String METHOD_DEFAULT = "0";
    private static final String METHOD_DOUBLETAP = "1";
    private static final String METHOD_SWEEP = "2";
    private static final String METHOD_ALL = "3";

    public static boolean isSupported() {
        return Utils.fileExists(FILE_DT2W) && Utils.fileExists(FILE_S2W);
    }

    private static void setSysFsForMethod(String method)
    {
        if (method.equals(METHOD_DEFAULT))
        {
             Utils.writeValue(FILE_DT2W, "0\n");
             Utils.writeValue(FILE_S2W, "0\n");
        } else
        if (method.equals(METHOD_DOUBLETAP))
        {
             Utils.writeValue(FILE_DT2W, "1\n");
             Utils.writeValue(FILE_S2W, "0\n");
        } else
        if (method.equals(METHOD_SWEEP))
        {
             Utils.writeValue(FILE_DT2W, "0\n");
             Utils.writeValue(FILE_S2W, "1\n");
        }
        if (method.equals(METHOD_ALL))
        {
             Utils.writeValue(FILE_DT2W, "1\n");
             Utils.writeValue(FILE_S2W, "1\n");
        }
    }

    /**
     * Restore WakeMethod setting from SharedPreferences. (Write to kernel.)
     * @param context       The context to read the SharedPreferences from
     */
    public static void restore(Context context) {
        if (!isSupported()) {
            return;
        }

        SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(context);
        String method = sharedPrefs.getString(TouchscreenFragmentActivity.KEY_WAKE_METHOD, "0");
        setSysFsForMethod(method);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        setSysFsForMethod((String)newValue);
        return true;
    }

}
