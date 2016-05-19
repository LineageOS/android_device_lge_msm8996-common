#
# System Properties for LG G5
#

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
    audio.offload.buffer.size.kb=32 \
    audio.offload.pcm.16bit.enable=false \
    audio.offload.pcm.24bit.enable=true \
    audio.offload.24bit.enable=1 \
    audio.offload.pcm.enable=true \
    audio.offload.track.enable=true \
    audio.deep_buffer.media=true \
    audio.offload.multiaac.enable=true \
    audio.offload.gapless.enabled=true \
    audio.offload.multiple.enabled=true \
    audio.offload.video=false

# Data modules
PRODUCT_PROPERTY_OVERRIDES += \
    ro.use_data_netmgrd=true \
    persist.data.netmgrd.qos.enable=true \
    persist.data.mode=concurrent \

# Display
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qualcomm.cabl=0 \
    ro.sf.lcd_density=640 \
    sdm.perf_hint_window=50

# Fluence
PRODUCT_PROPERTY_OVERRIDES += \
    persist.audio.fluence.voicecall=true \
    persist.audio.fluence.voicerec=false \
    persist.audio.fluence.speaker=true

# OpenGLES
# 196608 is decimal for 0x30000 to report version 3.0
# 196609 is decimal for 0x30001 to report version 3.1
PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=196609

# Perf
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.extension_library=libqti-perfd-client.so

# RIL
PRODUCT_PROPERTY_OVERRIDES += \
    rild.libpath=/vendor/lib64/libril-qc-qmi-1.so \
    persist.radio.apm_sim_not_pwdn=1 \
    persist.radio.add_power_save=1 \
    persist.rild.nitz_plmn="" \
    persist.rild.nitz_long_ons_0="" \
    persist.rild.nitz_long_ons_1="" \
    persist.rild.nitz_long_ons_2="" \
    persist.rild.nitz_long_ons_3="" \
    persist.rild.nitz_short_ons_0="" \
    persist.rild.nitz_short_ons_1="" \
    persist.rild.nitz_short_ons_2="" \
    persist.rild.nitz_short_ons_3="" \
    ril.subscription.types=NV,RUIM \
    ro.ril.telephony.mqanelements=5 \
    ro.telephony.ril_class=LgeLteRIL \
    DEVICE_PROVISIONED=1

# Sensors
PRODUCT_PROPERTY_OVERRIDES += \
    persist.debug.sensors.hal=e \
    debug.qualcomm.sns.daemon=e \
    debug.qualcomm.sns.libsensor1=e \
    persist.sensors.mag_filter_size=8 \
    persist.sensors.knock_delay=1000 \
    persist.sensors.wul_multilevel=3 \
    persist.sensors.wul_thresh0=2 \
    persist.sensors.wul_thresh1=10 \
    persist.sensors.wul_thresh2=15 \
    persist.sensors.wul_delay=3000 \
    persist.sensors.onhand.en=0

# Time services
PRODUCT_PROPERTY_OVERRIDES += \
    persist.timed.enable=true

# WiFi
PRODUCT_PROPERTY_OVERRIDES += \
    wifi.interface=wlan0
