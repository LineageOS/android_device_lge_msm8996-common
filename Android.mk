#
# Copyright 2016 The CyanogenMod Project
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

ifneq ($(filter g5 h830 h850,$(TARGET_DEVICE)),)

LOCAL_PATH := $(call my-dir)

include $(call all-makefiles-under,$(LOCAL_PATH))

# ADSP Images
ADSP_IMAGES := \
    adsp.b00 adsp.b01 adsp.b02 adsp.b03 adsp.b04 adsp.b05 \
    adsp.b06 adsp.b07 adsp.b08 adsp.b09 adsp.b10 adsp.b11 adsp.mdt

ADSP_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(ADSP_IMAGES)))
$(ADSP_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "ADSP firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(ADSP_SYMLINKS)
# END ADSP Images

# CMN Images
CMN_IMAGES := \
    cmnlib.b00 cmnlib.b01 cmnlib.b02 cmnlib.b03 cmnlib.b04 cmnlib.b05 cmnlib.mdt \
    cmnlib64.b00 cmnlib64.b01 cmnlib64.b02 cmnlib64.b03 cmnlib64.b04 cmnlib64.b05 cmnlib64.mdt \

CMN_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(CMN_IMAGES)))
$(CMN_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "CMN firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(CMN_SYMLINKS)
# END CMN Images

# CPE Images
CPE_IMAGES := \
    cpe_9335.b08 cpe_9335.b09 cpe_9335.b11 cpe_9335.b14 cpe_9335.b16 cpe_9335.b18 cpe_9335.b19 \
    cpe_9335.b20 cpe_9335.b22 cpe_9335.b24 cpe_9335.b26 cpe_9335.b28 cpe_9335.b29 cpe_9335.mdt

CPE_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(CPE_IMAGES)))
$(CPE_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "CPE firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(CPE_SYMLINKS)
# END CPE Images

# CPPF Images
CPPF_IMAGES := \
    cppf.b00 cppf.b01 cppf.b02 cppf.b03 cppf.b04 \
    cppf.b05 cppf.b06 cppf.mdt

CPPF_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(CPPF_IMAGES)))
$(CPPF_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "CPPF firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /persist-lg/firmware/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(CPPF_SYMLINKS)
# END CPPF Images

# CSFIDO Images
CSFIDO_IMAGES := \
    csfido.b00 csfido.b01 csfido.b02 csfido.b03 csfido.b04 \
    csfido.b05 csfido.b06 csfido.mdt

CSFIDO_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(CSFIDO_IMAGES)))
$(CSFIDO_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "CSFIDO firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(CSFIDO_SYMLINKS)
# END CPPF Images

# DXHDCP2 Images
DXHDCP2_IMAGES := \
    dxhdcp2.b00 dxhdcp2.b01 dxhdcp2.b02 dxhdcp2.b03 dxhdcp2.b04 \
    dxhdcp2.b05 dxhdcp2.b06 dxhdcp2.mdt

DXHDCP2_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(DXHDCP2_IMAGES)))
$(DXHDCP2_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "DXHDCP2 firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(DXHDCP2_SYMLINKS)
# END DXHDCP2 Images

# FPCTZAPP  Images
FPCTZAPP_IMAGES := \
    fpctzapp.b00 fpctzapp.b01 fpctzapp.b02 fpctzapp.b03 fpctzapp.b04 \
    fpctzapp.b05 fpctzapp.b06 fpctzapp.mdt

FPCTZAPP_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(FPCTZAPP_IMAGES)))
$(FPCTZAPP_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "FPCTZAPP firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(FPCTZAPP_SYMLINKS)
# END FPCTZAPP Images

# HASHSTOR  Images
HASHSTOR_IMAGES := \
    hashstor.b00 hashstor.b01 hashstor.b02 hashstor.b03 hashstor.b04 \
    hashstor.b05 hashstor.b06 hashstor.mdt

HASHSTOR_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(HASHSTOR_IMAGES)))
$(HASHSTOR_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "HASHSTOR firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(HASHSTOR_SYMLINKS)
# END CPPF Images

# HDCP1  Images
HDCP1_IMAGES := \
    hdcp1.b00 hdcp1.b01 hdcp1.b02 hdcp1.b03 hdcp1.b04 \
    hdcp1.b05 hdcp1.b06 hdcp1.mdt

HDCP1_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(HDCP1_IMAGES)))
$(HDCP1_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "HDCP1 firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(HDCP1_SYMLINKS)
# END HDCP1 Images

# IMS Libraries
IMS_LIBS := libimscamera_jni.so libimsmedia_jni.so

IMS_SYMLINKS := $(addprefix $(TARGET_OUT_VENDOR_APPS)/ims/lib/arm64/,$(notdir $(IMS_LIBS)))
$(IMS_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "IMS lib link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /system/vendor/lib64/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(IMS_SYMLINKS)
#  End IMS Libraries

# ISDB Images
ISDB_IMAGES := \
    isdbtmm.b00 isdbtmm.b01 isdbtmm.b02 isdbtmm.b03 isdbtmm.b04 \
    isdbtmm.b05 isdbtmm.b06 isdbtmm.mdt

ISDB_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(ISDB_IMAGES)))
$(ISDB_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "ISDB firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(ISDB_SYMLINKS)
# END ISDB Images

# MLSERVER  Images
MLSERVER_IMAGES := \
    mlserver.b00 mlserver.b01 mlserver.b02 mlserver.b03 mlserver.b04 \
    mlserver.b05 mlserver.b06 mlserver.mdt

MLSERVER_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(MLSERVER_IMAGES)))
$(MLSERVER_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "MLSERVER firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(MLSERVER_SYMLINKS)
# END MLSERVER Images

# QMPSECAP Images
QMPSECAP_IMAGES := \
    qmpsecap.b00 qmpsecap.b01 qmpsecap.b02 qmpsecap.b03 qmpsecap.b04 \
    qmpsecap.b05 qmpsecap.b06 qmpsecap.mdt

QMPSECAP_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(QMPSECAP_IMAGES)))
$(QMPSECAP_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "QMPSECAP firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(QMPSECAP_SYMLINKS)
# END QMPSECAP Images

# SECUREKS Images
SECUREKS_IMAGES := \
    secureks.b00 secureks.b01 secureks.b02 secureks.b03 secureks.b04 \
    secureks.b05 secureks.b06 secureks.mdt

SECUREKS_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(SECUREKS_IMAGES)))
$(SECUREKS_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "SECUREKS firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(SECUREKS_SYMLINKS)
# END SECUREKS Images

# SECUREMM Images
SECUREMM_IMAGES := \
    securemm.b00 securemm.b01 securemm.b02 securemm.b03 securemm.b04 \
    securemm.b05 securemm.b06 securemm.mdt

SECUREMM_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(SECUREMM_IMAGES)))
$(SECUREMM_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "SECUREMM firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(SECUREMM_SYMLINKS)
# END SECUREMM Images

# SLPI Images
SLPI_IMAGES := \
    slpi.b00 slpi.b01 slpi.b02 slpi.b03 slpi.b04 slpi.b05 \
    slpi.b06 slpi.b07 slpi.b08 slpi.b09 slpi.b10 slpi.b11 \
    slpi.b12 slpi.b13 slpi.b14 slpi.b15 slpi.mdt

SLPI_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(SLPI_IMAGES)))
$(SLPI_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "SLPI firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(SLPI_SYMLINKS)
# END SLPI Images

# STOOL Images
STOOL_IMAGES := \
    stool.b00 stool.b01 stool.b02 stool.b03 stool.b04 \
    stool.b05 stool.b06 stool.mdt

STOOL_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(STOOL_IMAGES)))
$(STOOL_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "STOOL firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(STOOL_SYMLINKS)
# END STOOL Images

# TBASE Images
TBASE_IMAGES := \
    tbase.b00 tbase.b01 tbase.b02 tbase.b03 tbase.b04 \
    tbase.b05 tbase.b06 tbase.mdt

TBASE_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(TBASE_IMAGES)))
$(TBASE_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "TBASE firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(TBASE_SYMLINKS)
# END WIDEVINE Images

# VENUS Images
VENUS_IMAGES := \
    venus.b00 venus.b01 venus.b02 venus.b03 venus.b04 \
    venus.mdt

VENUS_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(VENUS_IMAGES)))
$(VENUS_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "VENUS firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(VENUS_SYMLINKS)
# END VENUS Images

# WIDEVINE Images
WIDEVINE_IMAGES := \
    widevine.b00 widevine.b01 widevine.b02 widevine.b03 widevine.b04 \
    widevine.b05 widevine.b06 widevine.mdt

WIDEVINE_SYMLINKS := $(addprefix $(TARGET_OUT_ETC)/firmware/,$(notdir $(WIDEVINE_IMAGES)))
$(WIDEVINE_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "WIDEVINE firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /persist-lg/firmware/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(WIDEVINE_SYMLINKS)
# END WIDEVINE Images

# Same as for wcd9306 files
$(shell mkdir -p $(TARGET_OUT)/etc/firmware/wcd9306; \
    ln -sf /data/misc/audio/wcd9320_anc.bin \
	    $(TARGET_OUT)/etc/firmware/wcd9306/wcd9306_anc.bin; \
    ln -sf /data/misc/audio/mbhc.bin \
	    $(TARGET_OUT)/etc/firmware/wcd9306/wcd9306_mbhc.bin;)

endif
