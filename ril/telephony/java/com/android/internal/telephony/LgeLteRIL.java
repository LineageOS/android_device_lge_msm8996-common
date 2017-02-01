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

package com.android.internal.telephony;

import static com.android.internal.telephony.RILConstants.*;

import android.content.Context;
import android.os.AsyncResult;
import android.os.Message;
import android.os.Parcel;
import android.os.SystemProperties;
import android.util.Log;

import com.android.internal.telephony.RILConstants;

import com.android.internal.telephony.uicc.IccCardApplicationStatus;
import com.android.internal.telephony.uicc.IccCardStatus;

/**
 * Custom Qualcomm RIL for LG G5
 *
 * {@hide}
 */
public class LgeLteRIL extends RIL implements CommandsInterface {
    static final String LOG_TAG = "LgeLteRIL";

    public static final int RIL_UNSOL_AVAILABLE_RAT = 1054;
    public static final int RIL_UNSOL_LOG_RF_BAND_INFO = 1165;
    public static final int RIL_UNSOL_LTE_REJECT_CAUSE = 1187;

    public LgeLteRIL(Context context, int preferredNetworkType, int cdmaSubscription) {
        super(context, preferredNetworkType, cdmaSubscription, null);
    }

    public LgeLteRIL(Context context, int preferredNetworkType,
            int cdmaSubscription, Integer instanceId) {
        super(context, preferredNetworkType, cdmaSubscription, instanceId);
    }

    static String
    lgeResponseToString(int request)
    {
        switch(request) {
            case RIL_UNSOL_AVAILABLE_RAT: return "RIL_UNSOL_AVAILABLE_RAT";
            case RIL_UNSOL_LOG_RF_BAND_INFO: return "RIL_UNSOL_LOG_RF_BAND_INFO";
            case RIL_UNSOL_LTE_REJECT_CAUSE: return "RIL_UNSOL_LTE_REJECT_CAUSE";
            default: return "<unknown response>";
        }
    }

    protected void lgeUnsljLogRet(int response, Object ret) {
        riljLog("[LGE-UNSL]< " + lgeResponseToString(response) + " " + retToString(response, ret));
    }

    @Override
    protected void
    processUnsolicited (Parcel p, int type) {
        Object ret;
        int dataPosition = p.dataPosition(); // save off position within the Parcel
        int response = p.readInt();

        switch(response) {
            case RIL_UNSOL_AVAILABLE_RAT: ret = responseInts(p); break;
            case RIL_UNSOL_LOG_RF_BAND_INFO: ret = responseInts(p); break;
            case RIL_UNSOL_LTE_REJECT_CAUSE: ret = responseInts(p); break;
            default:
                // Rewind the Parcel
                p.setDataPosition(dataPosition);
                // Forward responses that we are not overriding to the super class
                super.processUnsolicited(p, type);
                return;
        }

        switch(response) {
            case RIL_UNSOL_AVAILABLE_RAT:
                if (RILJ_LOGD) lgeUnsljLogRet(response, ret);
                break;
            case RIL_UNSOL_LOG_RF_BAND_INFO:
                if (RILJ_LOGD) lgeUnsljLogRet(response, ret);
                break;
            case RIL_UNSOL_LTE_REJECT_CAUSE:
                if (RILJ_LOGD) lgeUnsljLogRet(response, ret);
                break;
        }
    }

    @Override
    protected Object
    responseIccCardStatus(Parcel p) {
        IccCardApplicationStatus appStatus = null;

        IccCardStatus cardStatus = new IccCardStatus();
        cardStatus.setCardState(p.readInt());
        cardStatus.setUniversalPinState(p.readInt());
        cardStatus.mGsmUmtsSubscriptionAppIndex = p.readInt();
        cardStatus.mCdmaSubscriptionAppIndex = p.readInt();
        cardStatus.mImsSubscriptionAppIndex = p.readInt();

        int numApplications = p.readInt();

        // limit to maximum allowed applications
        if (numApplications > IccCardStatus.CARD_MAX_APPS) {
            numApplications = IccCardStatus.CARD_MAX_APPS;
        }
        cardStatus.mApplications = new IccCardApplicationStatus[numApplications];

        for (int i = 0 ; i < numApplications ; i++) {
            appStatus = new IccCardApplicationStatus();
            appStatus.app_type       = appStatus.AppTypeFromRILInt(p.readInt());
            appStatus.app_state      = appStatus.AppStateFromRILInt(p.readInt());
            appStatus.perso_substate = appStatus.PersoSubstateFromRILInt(p.readInt());
            appStatus.aid            = p.readString();
            appStatus.app_label      = p.readString();
            appStatus.pin1_replaced  = p.readInt();
            appStatus.pin1           = appStatus.PinStateFromRILInt(p.readInt());
            appStatus.pin2           = appStatus.PinStateFromRILInt(p.readInt());
            int remaining_count_pin1 = p.readInt();
            int remaining_count_puk1 = p.readInt();
            int remaining_count_pin2 = p.readInt();
            int remaining_count_puk2 = p.readInt();
            cardStatus.mApplications[i] = appStatus;
        }

        if (numApplications == 1 && appStatus != null
                && appStatus.app_type == appStatus.AppTypeFromRILInt(2)) {
            cardStatus.mApplications = new IccCardApplicationStatus[numApplications + 2];
            cardStatus.mGsmUmtsSubscriptionAppIndex = 0;
            cardStatus.mApplications[cardStatus.mGsmUmtsSubscriptionAppIndex] = appStatus;
            cardStatus.mCdmaSubscriptionAppIndex = 1;
            cardStatus.mImsSubscriptionAppIndex = 2;
            IccCardApplicationStatus appStatus2 = new IccCardApplicationStatus();
            appStatus2.app_type       = appStatus2.AppTypeFromRILInt(4); // CSIM State
            appStatus2.app_state      = appStatus.app_state;
            appStatus2.perso_substate = appStatus.perso_substate;
            appStatus2.aid            = appStatus.aid;
            appStatus2.app_label      = appStatus.app_label;
            appStatus2.pin1_replaced  = appStatus.pin1_replaced;
            appStatus2.pin1           = appStatus.pin1;
            appStatus2.pin2           = appStatus.pin2;
            cardStatus.mApplications[cardStatus.mCdmaSubscriptionAppIndex] = appStatus2;
            IccCardApplicationStatus appStatus3 = new IccCardApplicationStatus();
            appStatus3.app_type       = appStatus3.AppTypeFromRILInt(5); // IMS State
            appStatus3.app_state      = appStatus.app_state;
            appStatus3.perso_substate = appStatus.perso_substate;
            appStatus3.aid            = appStatus.aid;
            appStatus3.app_label      = appStatus.app_label;
            appStatus3.pin1_replaced  = appStatus.pin1_replaced;
            appStatus3.pin1           = appStatus.pin1;
            appStatus3.pin2           = appStatus.pin2;
            cardStatus.mApplications[cardStatus.mImsSubscriptionAppIndex] = appStatus3;
        }

        return cardStatus;
    }

    @Override
    public void
    setNetworkSelectionModeManual(String operatorNumeric, Message response) {
        RILRequest rr
                = RILRequest.obtain(RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL,
                                    response);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest)
                    + " " + operatorNumeric);

        rr.mParcel.writeInt(2);
        rr.mParcel.writeString(operatorNumeric);
        rr.mParcel.writeString("2"); // NOCHANGE

        send(rr);
    }
}
