LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -DUSES_BOOTDEVICE_PATH

LOCAL_C_INCLUDES := \
    bootable/recovery/edify/include \
    bootable/recovery/otautil/include \
    bootable/recovery/updater/include \
    system/core/base/include
LOCAL_SRC_FILES := recovery_updater.cpp
LOCAL_MODULE := librecovery_updater_msm8996
include $(BUILD_STATIC_LIBRARY)
