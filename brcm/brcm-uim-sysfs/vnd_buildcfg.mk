ifneq ($(BOARD_CUSTOM_FM_CONFIG),)
generated_sources := $(local-generated-sources-dir)

SRC := $(BOARD_CUSTOM_FM_CONFIG)
GEN := $(generated_sources)/vnd_buildcfg.h
TOOL := $(LOCAL_PATH)/gen-buildcfg.sh

# Avoid -Wmacro-redefined error if using
# config file with SYSFS_PREFIX defined.
CHKPREFIX := $(shell cat $(SRC) | grep -o SYSFS_PREFIX)
ifeq ($(CHKPREFIX),SYSFS_PREFIX)
LOCAL_CFLAGS := $(filter-out -DSYSFS_PREFIX=%,$(LOCAL_CFLAGS))
endif

$(GEN): PRIVATE_PATH := $(call my-dir)
$(GEN): PRIVATE_CUSTOM_TOOL = $(TOOL) $< $@
$(GEN): $(SRC)  $(TOOL)
	$(transform-generated-source)

LOCAL_GENERATED_SOURCES += $(GEN)

endif
