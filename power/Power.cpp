/*
 * Copyright (C) 2017 The Android Open Source Project
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

#define LOG_TAG "android.hardware.power@1.1-service.lge.msm8996"

#include <android/log.h>
#include <utils/Log.h>

#include <android-base/properties.h>

#include "Power.h"
#include "power-helper.h"

/* RPM runs at 19.2Mhz. Divide by 19200 for msec */
#define RPM_CLK 19200

extern struct stat_pair rpm_stat_map[];

namespace android {
namespace hardware {
namespace power {
namespace V1_1 {
namespace implementation {

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_0::PowerHint;
using ::android::hardware::power::V1_0::PowerStatePlatformSleepState;
using ::android::hardware::power::V1_0::Status;
using ::android::hardware::power::V1_1::PowerStateSubsystem;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

Power::Power() {
    power_init();
}

// Methods from ::android::hardware::power::V1_0::IPower follow.
Return<void> Power::setInteractive(bool interactive)  {
    power_set_interactive(interactive ? 1 : 0);
    return Void();
}

Return<void> Power::powerHint(PowerHint hint, int32_t data) {
    if (android::base::GetProperty("init.svc.vendor.perfd", "") != "running") {
        ALOGW("perfd is not started");
        return Void();
    }
    power_hint(static_cast<power_hint_t>(hint), data ? (&data) : NULL);
    return Void();
}

Return<void> Power::setFeature(Feature /*feature*/, bool /*activate*/)  {
    return Void();
}

Return<void> Power::getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb) {

    hidl_vec<PowerStatePlatformSleepState> states;
    uint64_t stats[platform_param_id::PLATFORM_PARAM_COUNT] = {0};
    struct PowerStatePlatformSleepState *state;
    int ret;

    ret = extract_platform_stats(stats);
    if (ret != 0) {
        states.resize(0);
        goto done;
    }

    states.resize(platform_mode_id::RPM_MODE_COUNT);

    /* Update statistics for XO_shutdown */
    state = &states[platform_mode_id::RPM_MODE_XO];
    state->name = "XO_shutdown";

    state->residencyInMsecSinceBoot = stats[platform_param_id::ACCUMULATED_VLOW_TIME];
    state->totalTransitions = stats[platform_param_id::VLOW_COUNT];
    state->supportedOnlyInSuspend = false;
    state->voters.resize(XO_VOTERS);

    /* Update statistics for APSS voter */
    state->voters[0].name = "APSS";
    state->voters[0].totalTimeInMsecVotedForSinceBoot =
        stats[platform_param_id::XO_ACCUMULATED_DURATION_APSS] / RPM_CLK;
    state->voters[0].totalNumberOfTimesVotedSinceBoot = stats[platform_param_id::XO_COUNT_APSS];

    /* Update statistics for MPSS voter */
    state->voters[1].name = "MPSS";
    state->voters[1].totalTimeInMsecVotedForSinceBoot =
        stats[platform_param_id::XO_ACCUMULATED_DURATION_MPSS] / RPM_CLK;
    state->voters[1].totalNumberOfTimesVotedSinceBoot = stats[platform_param_id::XO_COUNT_MPSS];

    /* Update statistics for ADSP voter */
    state->voters[2].name = "ADSP";
    state->voters[2].totalTimeInMsecVotedForSinceBoot =
        stats[platform_param_id::XO_ACCUMULATED_DURATION_ADSP] / RPM_CLK;
    state->voters[2].totalNumberOfTimesVotedSinceBoot = stats[platform_param_id::XO_COUNT_ADSP];

    /* Update statistics for SLPI voter */
    state->voters[3].name = "SLPI";
    state->voters[3].totalTimeInMsecVotedForSinceBoot =
        stats[platform_param_id::XO_ACCUMULATED_DURATION_SLPI] / RPM_CLK;
    state->voters[3].totalNumberOfTimesVotedSinceBoot = stats[platform_param_id::XO_COUNT_SLPI];


    /* Update statistics for VMIN state */
    state = &states[platform_mode_id::RPM_MODE_VMIN];
    state->name = "VMIN";

    state->residencyInMsecSinceBoot = stats[platform_param_id::ACCUMULATED_VMIN_TIME];
    state->totalTransitions = stats[platform_param_id::VMIN_COUNT];
    state->supportedOnlyInSuspend = false;
    state->voters.resize(VMIN_VOTERS);
    //Note: No filling of state voters since VMIN_VOTERS = 0

done:
    _hidl_cb(states, Status::SUCCESS);
    return Void();
}


// Methods from ::android::hardware::power::V1_1::IPower follow.

Return<void> Power::getSubsystemLowPowerStats(getSubsystemLowPowerStats_cb _hidl_cb) {

    hidl_vec<PowerStateSubsystem> subsystems;
    int ret;

done:
    _hidl_cb(subsystems, Status::SUCCESS);
    return Void();
}

Return<void> Power::powerHintAsync(PowerHint hint, int32_t data) {
    // just call the normal power hint in this oneway function
    return powerHint(hint, data);
}

}  // namespace implementation
}  // namespace V1_1
}  // namespace power
}  // namespace hardware
}  // namespace android
