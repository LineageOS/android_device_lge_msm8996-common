#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set -e

export INITIAL_COPYRIGHT_YEAR=2016
export G5_DEVICE_LIST="g5 h830 h850 rs988"
export V20_DEVICE_LIST="v20 h910 h915 h918 h990 vs995 us996 ls997"
export G6_DEVICE_LIST="g6 h870 h872 us997"

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$MY_DIR" ]]; then MY_DIR="$PWD"; fi

LINEAGE_ROOT="$MY_DIR"/../../..

HELPER="$LINEAGE_ROOT"/vendor/lineage/build/tools/extract_utils.sh
if [ ! -f "$HELPER" ]; then
    echo "Unable to find helper script at $HELPER"
    exit 1
fi
. "$HELPER"

# Initialize the helper for common platform
setup_vendor "$PLATFORM_COMMON" "$VENDOR" "$LINEAGE_ROOT" true

# Copyright headers and common guards
BKP_DEVICE_COMMON="$DEVICE_COMMON"
DEVICE_COMMON="$PLATFORM_COMMON"
write_headers "$G5_DEVICE_LIST $V20_DEVICE_LIST $G6_DEVICE_LIST"
DEVICE_COMMON="$BKP_DEVICE_COMMON"

# The standard blobs
write_makefiles "$MY_DIR"/proprietary-files.txt true
write_makefiles "$MY_DIR"/proprietary-files-qc.txt true

# We are done with platform
write_footers

# Reinitialize the helper for common device
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$LINEAGE_ROOT" true

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

write_makefiles "$MY_DIR/../$DEVICE_COMMON/proprietary-files.txt" true

# We are done with common
write_footers

# Reinitialize the helper for device
setup_vendor "$DEVICE" "$VENDOR" "$LINEAGE_ROOT"

# Copyright headers and guards
write_headers

# Device specific blobs
write_makefiles "$MY_DIR/../$DEVICE/proprietary-files.txt" true
write_makefiles "$MY_DIR/../$DEVICE/proprietary-files-qc.txt" true

# We are done with device
write_footers
