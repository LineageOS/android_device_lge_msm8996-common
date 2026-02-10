/*
 * Copyright (C) 2013 The Android Open Source Project
 * Copyright (C) 2017-2018 The LineageOS Project
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
#define LOG_TAG "ConsumerIrService"

#include <fcntl.h>
#include <linux/lirc.h>

#include <log/log.h>

#include "ConsumerIr.h"

namespace android {
namespace hardware {
namespace ir {
namespace V1_0 {
namespace implementation {

// LG specific defines
#define IR_DEVICE "/dev/ttyMSM1"
#define LG_IR_BAUD_RATE 115200

extern "C" {
extern int transmitIr(const char *dev, int baudRate, int frequency, int pattern[], int pattern_len);
}

static hidl_vec<ConsumerIrFreqRange> rangeVec{
    {.min = 25000, .max = 125000},
};

enum LG_TRANSMIT_IR_RETURN_CODES {
    IR_SUCCESS,
    IR_FAIL,
    IR_INVALID_PORT,
    IR_INVALID_BAUDRATE,
    IR_ERROR_OPEN_PORT,
    IR_ERROR_WRITE,
    IR_ERROR_READ,
    IR_INVALID_DATA_SIZE,
    IR_INVALID_DATA,
    IR_MAX_DURATIONS_EXCEEDED
};

ConsumerIr::ConsumerIr() {}

Return<bool> ConsumerIr::transmit(int32_t carrierFreq, const hidl_vec<int32_t>& pattern) {
    size_t entries = pattern.size();
    int rc;

    // call into libcir_driver
    ALOGD("transmitting pattern at %d Hz", carrierFreq);
    rc = transmitIr(IR_DEVICE, LG_IR_BAUD_RATE, carrierFreq, const_cast<int32_t*>(pattern.data()), sizeof(int32_t) * entries);
    if (rc != IR_SUCCESS) {
        ALOGE("transmitIr() failed, error %d\n", rc);
        return false;
    }
    return true;
}

Return<void> ConsumerIr::getCarrierFreqs(getCarrierFreqs_cb _hidl_cb) {
    _hidl_cb(true, rangeVec);
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace ir
}  // namespace hardware
}  // namespace android
