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
import android.os.Message;
import android.os.Parcel;

/**
 * Custom Qualcomm RIL for G4
 *
 * {@hide}
 */
public class LgeLteRIL extends RIL implements CommandsInterface {

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
    processUnsolicited (Parcel p) {
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
                super.processUnsolicited(p);
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
