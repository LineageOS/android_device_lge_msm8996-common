/*
 * Copyright (c) 2015 The CyanogenMod Project
 *               2017-2018 The LineageOS Project
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

package org.lineageos.settings.doze;

import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.database.ContentObserver;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import androidx.preference.Preference;
import androidx.preference.Preference.OnPreferenceChangeListener;
import androidx.preference.PreferenceCategory;
import androidx.preference.PreferenceFragment;
import androidx.preference.SwitchPreference;

public class DozeSettingsFragment extends PreferenceFragment implements OnPreferenceChangeListener,
        CompoundButton.OnCheckedChangeListener {

    private TextView mTextView;

    private Switch mSwitch;

    private SwitchPreference mPickUpPreference;
    private SwitchPreference mTiltAlwaysPreference;
    private SwitchPreference mHandwavePreference;
    private SwitchPreference mPocketPreference;
    private SwitchPreference mProximityAlwaysPreference;

    private ContentObserver mDozeObserver = new ContentObserver(new Handler()) {
        @Override
        public void onChange(boolean selfChange) {
            super.onChange(selfChange);

            boolean enabled = Utils.isDozeEnabled(getActivity());

            updateSwitches(Utils.isDozeEnabled(getActivity()));
            DozeReceiver.notifyChanged(getActivity());
        }
    };

    static String getDozeSummary(Context context) {
        if (Utils.isDozeEnabled(context)) {
            return context.getString(R.string.ambient_display_summary_on);
        }
        return context.getString(R.string.ambient_display_summary_off);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        getActivity().getActionBar().setDisplayHomeAsUpEnabled(true);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        final View view = LayoutInflater.from(getContext()).inflate(R.layout.doze, container, false);
        ((ViewGroup) view).addView(super.onCreateView(inflater, container, savedInstanceState));
        return view;
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        addPreferencesFromResource(R.xml.doze_settings);
        final ActionBar actionBar = getActivity().getActionBar();
        actionBar.setDisplayHomeAsUpEnabled(true);

        if (savedInstanceState == null && !getActivity()
                .getSharedPreferences("doze_settings", Activity.MODE_PRIVATE)
                .getBoolean("first_help_shown", false)) {
            showHelp();
        }

        mPickUpPreference =
                (SwitchPreference) findPreference(Utils.PICK_UP_KEY);
        mPickUpPreference.setOnPreferenceChangeListener(this);

        mTiltAlwaysPreference =
                (SwitchPreference) findPreference(Utils.TILT_ALWAYS_KEY);
        mTiltAlwaysPreference.setOnPreferenceChangeListener(this);

        mHandwavePreference =
                (SwitchPreference) findPreference(Utils.GESTURE_HAND_WAVE_KEY);
        mHandwavePreference.setOnPreferenceChangeListener(this);

        mPocketPreference =
                (SwitchPreference) findPreference(Utils.GESTURE_POCKET_KEY);
        mPocketPreference.setOnPreferenceChangeListener(this);

        mProximityAlwaysPreference =
                (SwitchPreference) findPreference(Utils.PROXIMITY_ALWAYS_KEY);
        mProximityAlwaysPreference.setOnPreferenceChangeListener(this);
    }

    @Override
    public void onResume() {
        super.onResume();
        getActivity().getContentResolver().registerContentObserver(
                Utils.DOZE_ENABLED_URI, false, mDozeObserver);
        updateSwitches(Utils.isDozeEnabled(getActivity()));
    }

    @Override
    public void onPause() {
        super.onPause();
        getActivity().getContentResolver().unregisterContentObserver(mDozeObserver);
    }

    private void updateSwitches(boolean enabled) {
        mPickUpPreference.setEnabled(enabled);
        mHandwavePreference.setEnabled(enabled);
        mPocketPreference.setEnabled(enabled);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        boolean dozeEnabled = Utils.isDozeEnabled(getActivity());

        mTextView = view.findViewById(R.id.switch_text);
        mTextView.setText(getString(dozeEnabled ?
                R.string.switch_bar_on : R.string.switch_bar_off));

        View switchBar = view.findViewById(R.id.switch_bar);
        Switch switchWidget = switchBar.findViewById(android.R.id.switch_widget);
        switchWidget.setChecked(dozeEnabled);
        switchWidget.setOnCheckedChangeListener(this);
        switchBar.setOnClickListener(v -> switchWidget.setChecked(!switchWidget.isChecked()));
    }


    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        final String key = preference.getKey();
        final boolean value = (Boolean) newValue;
        if (Utils.PICK_UP_KEY.equals(key)) {
            mPickUpPreference.setChecked(value);
        } else if (Utils.TILT_ALWAYS_KEY.equals(key)) {
            mTiltAlwaysPreference.setChecked(value);
        } else if (Utils.GESTURE_HAND_WAVE_KEY.equals(key)) {
            mHandwavePreference.setChecked(value);
        } else if (Utils.GESTURE_POCKET_KEY.equals(key)) {
            mPocketPreference.setChecked(value);
        } else if (Utils.PROXIMITY_ALWAYS_KEY.equals(key)) {
            mProximityAlwaysPreference.setChecked(value);
        } else {
            return false;
        }

        Utils.startService(getActivity());
        return true;
    }

    @Override
    public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
        Utils.enableDoze(b, getActivity());
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            getActivity().onBackPressed();
            return true;
        }
        return false;
    }

    public static class HelpDialogFragment extends DialogFragment {
        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {
            return new AlertDialog.Builder(getActivity())
                    .setTitle(R.string.doze_settings_help_title)
                    .setMessage(R.string.doze_settings_help_text)
                    .setNegativeButton(R.string.dialog_ok, (dialog, which) -> dialog.cancel())
                    .create();
        }

        @Override
        public void onCancel(DialogInterface dialog) {
            getActivity().getSharedPreferences("doze_settings", Activity.MODE_PRIVATE)
                    .edit()
                    .putBoolean("first_help_shown", true)
                    .commit();
        }
    }

    private void showHelp() {
        HelpDialogFragment fragment = new HelpDialogFragment();
        fragment.show(getFragmentManager(), "help_dialog");
    }
}
