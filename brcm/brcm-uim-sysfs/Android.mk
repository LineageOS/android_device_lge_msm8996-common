LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#
# UIM Application
#

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include

LOCAL_SRC_FILES:= \
    uim.c \
    upio.c \
    brcm_hci_dump.c \
    btsnoop.c \
    utils.c

LOCAL_CFLAGS:= -c -W -Wall -O2 -D_POSIX_SOURCE -DUIM_DEBUG -DBLUEDROID_ENABLE_V4L2
LOCAL_SHARED_LIBRARIES:= libnetutils libcutils liblog

SYSFS_PREFIX := "/sys/bus/platform/drivers/bcm_ldisc/bcmbt_ldisc.93/"
SYSFS_PREFIX_V4L2 := "/sys/class/video4linux/radio0/"
ifneq ($(BOARD_HAVE_BCM_FM_SYSFS),)
SYSFS_PREFIX := $(BOARD_HAVE_BCM_FM_SYSFS)
endif
LOCAL_CFLAGS += -DSYSFS_PREFIX=\"$(SYSFS_PREFIX)\"
LOCAL_CFLAGS += -DSYSFS_PREFIX_V4L2=\"$(SYSFS_PREFIX_V4L2)\"

ifeq ($(BOARD_HAVE_LGE_BLUESLEEP),true)
LOCAL_CFLAGS += -DLGE_BLUESLEEP_PM
LOCAL_CFLAGS += -DLGE_RECOVERY_BLOCK_READ
LOCAL_CFLAGS += -DLGE_PROC_PREPROTO=\"/proc/bluetooth/sleep/preproto\"
endif

ifneq ($(BOARD_BRCM_HCI_NUM),)
LOCAL_CFLAGS += -DBOARD_BRCM_HCI_NUM=$(BOARD_BRCM_HCI_NUM)
endif

LOCAL_MODULE := brcm-uim-sysfs
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := sony
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_PROPRIETARY_MODULE := true

include $(LOCAL_PATH)/vnd_buildcfg.mk
include $(BUILD_EXECUTABLE)
