#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

export G5_DEVICE_LIST="g5 h830 h850 rs988"
export V20_DEVICE_LIST="v20 h910 h915 h918 h990 vs995 us996 us996d ls997"
export G6_DEVICE_LIST="g6 h870 h870d h872 h873 us997"

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$MY_DIR" ]]; then MY_DIR="$PWD"; fi

ANDROID_ROOT="$MY_DIR"/../../..

HELPER="${ANDROID_ROOT}/tools/extract-utils/extract_utils.sh"
if [ ! -f "$HELPER" ]; then
    echo "Unable to find helper script at $HELPER"
    exit 1
fi
. "$HELPER"

function vendor_imports() {
    cat <<EOF >>"$1"
                "device/lge/msm8996-common",
                "hardware/lge",
                "hardware/qcom-caf/msm8996",
                "hardware/qcom-caf/wlan",
                "vendor/qcom/opensource/dataservices",
                "vendor/qcom/opensource/display",
EOF
}

function lib_to_package_fixup_vendor_variants() {
    if [ "$2" != "vendor" ]; then
        return 1
    fi
    case "$1" in
        com.qualcomm.qti.dpm.api@1.0)
            echo "$1_vendor"
            ;;
        libgps.utils| \
            liblbs_core| \
            libloc_core| \
            libloc_api_v02| \
            libloc_pla| \
            liblocation_api)
            ;;
        *)
            return 1
            ;;
    esac
}

function lib_to_package_fixup() {
    lib_to_package_fixup_clang_rt_ubsan_standalone "$1" ||
        lib_to_package_fixup_proto_3_9_1 "$1" ||
        lib_to_package_fixup_vendor_variants "$@"
}


# Initialize the helper for common platform
setup_vendor "$PLATFORM_COMMON" "$VENDOR" "$ANDROID_ROOT" true

# Copyright headers and common guards
BKP_DEVICE_COMMON="$DEVICE_COMMON"
DEVICE_COMMON="$PLATFORM_COMMON"
write_headers "$G5_DEVICE_LIST $V20_DEVICE_LIST $G6_DEVICE_LIST"
DEVICE_COMMON="$BKP_DEVICE_COMMON"

# The standard blobs
write_makefiles "$MY_DIR"/proprietary-files.txt true

# We are done with platform
write_footers

# Reinitialize the helper for common device
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$ANDROID_ROOT" true

# Copyright headers and guards
case "$DEVICE_COMMON" in
g5-common)
    write_headers "$G5_DEVICE_LIST"
;;
g6-common)
    write_headers "$G6_DEVICE_LIST"
;;
v20-common)
    write_headers "$V20_DEVICE_LIST"
;;
*)
    printf 'Unknown device common: "%s"\n' "$DEVICE_COMMON"
    exit 1
;;
esac

write_makefiles "$MY_DIR/../$DEVICE_COMMON/proprietary-files.txt"

grep -q '"vendor/lge/msm8996-common"' ../../../vendor/lge/"$DEVICE_COMMON"/Android.bp || \
sed -i '/imports: \[/a\                "vendor/lge/msm8996-common",' ../../../vendor/lge/"$DEVICE_COMMON"/Android.bp

# We are done with common
write_footers

# Reinitialize the helper for device
setup_vendor "$DEVICE" "$VENDOR" "$ANDROID_ROOT"

# Copyright headers and guards
write_headers

# Device specific blobs
write_makefiles "$MY_DIR/../$DEVICE/proprietary-files.txt"

# We are done with device
write_footers
