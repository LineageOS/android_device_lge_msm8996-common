#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$MY_DIR" ]]; then MY_DIR="$PWD"; fi

ANDROID_ROOT="$MY_DIR/../../.."

export TARGET_ENABLE_CHECKELF=false

HELPER="${ANDROID_ROOT}/tools/extract-utils/extract_utils.sh"
if [ ! -f "$HELPER" ]; then
    echo "Unable to find helper script at $HELPER"
    exit 1
fi
. "$HELPER"

function blob_fixup() {
    case "${1}" in
    system/etc/permissions/qti_libpermissions.xml)
        sed -i "s/name=\"android.hidl.manager-V1.0-java/name=\"android.hidl.manager@1.0-java/g" "${2}"
        ;;
    system/lib/libdovi.so|system/lib64/libdovi.so)
        "${PATCHELF_0_18}" --add-needed "libgui_shim.so" "${2}"
        ;;
    system/lib/libkeystore_binder.so|system/lib64/libkeystore_binder.so)
        "${PATCHELF_0_18}" --replace-needed "libbinder.so" "libbinder-v32.so" "${2}"
        "${PATCHELF_0_18}" --replace-needed "libprotobuf-cpp-lite.so" "libprotobuf-cpp-lite-v29.so" "${2}"
        ;;
    system/lib/liblgkm.so|system/lib64/liblgkm.so)
        grep -q libbase_shim.so "${2}" || "${PATCHELF_0_18}" --add-needed "libbase_shim.so" "${2}"
        ;;
    system_ext/etc/init/dpmd.rc)
        sed -i "s/\/system\/product\/bin\//\/system\/system_ext\/bin\//g" "${2}"
        ;;
    system_ext/etc/permissions/com.qti.dpmframework.xml)
        ;&
    system_ext/etc/permissions/dpmapi.xml)
        sed -i "s/\/system\/product\/framework\//\/system\/system_ext\/framework\//g" "${2}"
        ;;
    system_ext/lib64/libdpmframework.so)
        sed -i "s/libhidltransport.so/libcutils-v29.so\x00\x00\x00/" "${2}"
        ;;
    vendor/bin/hw/vendor.display.color@1.0-service|vendor/lib/vendor.display.color@1.0.so|vendor/lib/vendor.display.color@1.1.so|vendor/lib/vendor.display.color@1.2.so|vendor/lib/vendor.display.postproc@1.0.so|vendor/lib/vendor.qti.hardware.radio.am@1.0_vendor.so|vendor/lib/vendor.qti.hardware.radio.atcmdfwd@1.0_vendor.so|vendor/lib/vendor.qti.hardware.radio.ims@1.0_vendor.so|vendor/lib/vendor.qti.hardware.radio.lpa@1.0_vendor.so|vendor/lib/vendor.qti.hardware.radio.qcrilhook@1.0_vendor.so|vendor/lib/vendor.qti.hardware.radio.qtiradio@1.0_vendor.so|vendor/lib/vendor.qti.hardware.radio.uim@1.0_vendor.so|vendor/lib/vendor.qti.hardware.radio.uim_remote_client@1.0_vendor.so|vendor/lib/vendor.qti.hardware.radio.uim_remote_server@1.0_vendor.so|vendor/lib/vendor.qti.hardware.tui_comm@1.0_vendor.so|vendor/lib64/android.system.net.netd@1.0_vendor.so|vendor/lib64/libril-qc-qmi-1.so|vendor/lib64/libsecureui_svcsock.so|vendor/lib64/vendor.display.color@1.0.so|vendor/lib64/vendor.display.color@1.1.so|vendor/lib64/vendor.display.color@1.2.so|vendor/lib64/vendor.display.postproc@1.0.so|vendor/lib64/vendor.qti.hardware.radio.am@1.0_vendor.so|vendor/lib64/vendor.qti.hardware.radio.atcmdfwd@1.0_vendor.so|vendor/lib64/vendor.qti.hardware.radio.ims@1.0_vendor.so|vendor/lib64/vendor.qti.hardware.radio.lpa@1.0_vendor.so|vendor/lib64/vendor.qti.hardware.radio.qcrilhook@1.0_vendor.so|vendor/lib64/vendor.qti.hardware.radio.qtiradio@1.0_vendor.so|vendor/lib64/vendor.qti.hardware.radio.uim@1.0_vendor.so|vendor/lib64/vendor.qti.hardware.radio.uim_remote_client@1.0_vendor.so|vendor/lib64/vendor.qti.hardware.radio.uim_remote_server@1.0_vendor.so|vendor/lib64/vendor.qti.hardware.tui_comm@1.0_vendor.so)
        "${PATCHELF_0_18}" --replace-needed "libhidlbase.so" "libhidlbase-v32.so" "${2}"
        ;;
    vendor/bin/pm-service)
        grep -q libutils-v33.so "${2}" || "${PATCHELF}" --add-needed "libutils-v33.so" "${2}"
        ;;
    vendor/lib/hw/camera.msm8996.so)
        sed -i "s/service.bootanim.exit/service.bootanim.zzzz/g" "${2}"
        ;;
    vendor/lib/libAutoContrast.so|vendor/lib/libCmcPdaf.so|vendor/lib/libSJFingerDetect.so|vendor/lib/libarcsoft_beauty_shot.so|vendor/lib/libarcsoft_object_tracking.so|vendor/lib/libchromaflash.so|vendor/lib/libcir_driver.so|vendor/lib/libfilm_emulation.so|vendor/lib/libHDR.so|vendor/lib/liblgmda.so|vendor/lib/liblghdri.so|vendor/lib/libmorpho_image_stab31.so|vendor/lib/libmorpho_superzoom.so|vendor/lib/libmpbase.so|vendor/lib/liboptizoom.so|vendor/lib/libseemore.so|vendor/lib/libtrueportrait.so|vendor/lib/libts_detected_face_hal.so|vendor/lib/libts_face_beautify_hal.so|vendor/lib/libubifocus.so|vendor/lib64/libcir_driver.so|vendor/lib64/libseemore.so|vendor/lib64/libts_detected_face_hal.so|vendor/lib64/libts_face_beautify_hal.so)
        "${PATCHELF_0_18}" --replace-needed "libstdc++.so" "libstdc++_vendor.so" "${2}"
        ;;
    vendor/lib/libdovi.so|vendor/lib64/libdovi.so)
        "${PATCHELF_0_18}" --add-needed "libgui_shim_vendor.so" "${2}"
        ;;
    vendor/lib/libmmcamera_hdr_gb_lib.so)
        "${PATCHELF_0_18}" --replace-needed "libstdc++.so" "libstdc++_vendor.so" "${2}"
        "${PATCHELF_0_18}" --add-needed "liblog.so" "${2}"
        ;;
    vendor/lib/libfpfactory_jni.so|vendor/lib/liblgae_main.so|vendor/lib/liblgawb_main.so|vendor/lib/libmmcamera_pdaf.so|vendor/lib/libmmcamera_pdafcamif.so|vendor/lib/libmmcamera_tintless_bg_pca_algo.so|vendor/lib64/libfpfactory_jni.so)
        "${PATCHELF_0_18}" --add-needed "liblog.so" "${2}"
        ;;
    vendor/lib/vulkan.msm8996.so)
        sed -i "s/vulkan.msm8953.so/vulkan.msm8996.so/g" "${2}"
        ;;
    vendor/lib64/libsettings.so)
        "${PATCHELF}" --replace-needed "libprotobuf-cpp-full.so" "libprotobuf-cpp-full-v29.so" "${2}"
        ;;
    vendor/lib64/libwvhidl.so|vendor/lib64/mediadrm/libwvdrmengine.so)
        "${PATCHELF}" --replace-needed "libprotobuf-cpp-lite.so" "libprotobuf-cpp-lite-v29.so" "${2}"
         grep -q libcrypto_shim.so "${2}" || "${PATCHELF}" --add-needed "libcrypto_shim.so" "${2}"
        ;;
    vendor/lib64/vulkan.msm8996.so)
        sed -i "s/vulkan.msm8953.so/vulkan.msm8996.so/g" "${2}"
        ;;
    esac
}

# Default to sanitizing the vendor folder before extraction
CLEAN_VENDOR=true

while [ "$1" != "" ]; do
    case $1 in
        -n | --no-cleanup )     CLEAN_VENDOR=false
                                ;;
        -s | --section )        shift
                                SECTION="$1"
                                CLEAN_VENDOR=false
                                ;;
        * )                     SRC="$1"
                                ;;
    esac
    shift
done

if [ -z "$SRC" ]; then
    SRC=adb
fi

# Initialize the helper for common platform
setup_vendor "$PLATFORM_COMMON" "$VENDOR" "$ANDROID_ROOT" true $CLEAN_VENDOR

extract "$MY_DIR"/proprietary-files.txt "$SRC" "$SECTION"

# Initialize the helper for common device
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$ANDROID_ROOT" true $CLEAN_VENDOR

extract "$MY_DIR/../$DEVICE_COMMON/proprietary-files.txt" "$SRC" "$SECTION"

# Reinitialize the helper for device
setup_vendor "$DEVICE" "$VENDOR" "$ANDROID_ROOT" false $CLEAN_VENDOR

extract "$MY_DIR/../$DEVICE/proprietary-files.txt" "$SRC" "$SECTION"

"$MY_DIR"/setup-makefiles.sh
