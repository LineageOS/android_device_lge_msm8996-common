/*
 * Copyright (C) 2016 The CyanogenMod Project
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

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.hardware.SensorEvent;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.SystemClock;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.util.Log;

import org.cyanogenmod.internal.util.FileUtils;

public class LGGestureService extends Service {

    private static final boolean DEBUG = false;

    public static final String TAG = "GestureService";

    private static final String LPWG_NOTIFY_SYSFS = "/sys/devices/virtual/input/lge_touch/lpwg_notify";

    private Context mContext;
    private PowerManager mPowerManager;

    @Override
    public void onCreate() {
        if (DEBUG) Log.d(TAG, "Creating service");
        super.onCreate();

        mContext = this;
        mPowerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (DEBUG) Log.d(TAG, "Starting service");
        IntentFilter screenStateFilter = new IntentFilter(Intent.ACTION_SCREEN_ON);
        screenStateFilter.addAction(Intent.ACTION_SCREEN_OFF);
        mContext.registerReceiver(mScreenStateReceiver, screenStateFilter);
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        if (DEBUG) Log.d(TAG, "Destroying service");
        super.onDestroy();
        unregisterReceiver(mScreenStateReceiver);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private boolean isDoubleTapEnabled() {
        return (Settings.Secure.getInt(mContext.getContentResolver(),
                    Settings.Secure.DOUBLE_TAP_TO_WAKE, 0) != 0);
    }

    private boolean writeLPWG(boolean state) {
        if (DEBUG) Log.d(TAG, "Writing to lpwg_notify");
        if (isDoubleTapEnabled()) {
            return FileUtils.writeLine(LPWG_NOTIFY_SYSFS, (state ? "9 1 1 1 0" : "9 1 0 1 0"));
        } else {
            return FileUtils.writeLine(LPWG_NOTIFY_SYSFS, (state ? "9 0 1 1 0" : "9 0 0 1 0"));
        }
    }

    private void onDisplayOn() {
        if (DEBUG) Log.d(TAG, "Display on");
        boolean display = true;
        writeLPWG(display);
    }

    private void onDisplayOff() {
        if (DEBUG) Log.d(TAG, "Display off");
        boolean display = false;
        writeLPWG(display);
    }

    private BroadcastReceiver mScreenStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
                onDisplayOff();
            } else if (intent.getAction().equals(Intent.ACTION_SCREEN_ON)) {
                onDisplayOn();
            }
        }
    };
}
