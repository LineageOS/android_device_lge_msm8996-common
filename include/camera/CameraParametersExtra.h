/*
 * Copyright (C) 2015 The CyanogenMod Project
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

#define CAMERA_PARAMETERS_EXTRA_C \
const char CameraParameters::BURST_SHOT_OFF[] = "burst-shot"; \
const char CameraParameters::BURST_SHOT_ON[] = "burst-shot"; \
const char CameraParameters::CAMERA_SWITCHING_VALUE_DEFAULT[] = "0"; \
const char CameraParameters::CAMERA_SWITCHING_VALUE_NORMAL_TO_WIDE[] = "2"; \
const char CameraParameters::CAMERA_SWITCHING_VALUE_SAME[] = "1"; \
const char CameraParameters::CAMERA_SWITCHING_VALUE_WIDE_TO_NORMAL[] = "3"; \
const char CameraParameters::CAMERA_WIDE_CAMERA_ZOOM_MAX[] = ""; \
const char CameraParameters::FLASH_MODE_REAR_ON[] = "flash-rear-on"; \
const char CameraParameters::FLIP_MODE_H[] = "flip-h"; \
const char CameraParameters::FLIP_MODE_OFF[] = "off"; \
const char CameraParameters::FLIP_MODE_V[] = "flip-v"; \
const char CameraParameters::FLIP_MODE_VH[] = "flip-vh"; \
const char CameraParameters::FOCUS_FACE_TRACKING[] = "face_tracking"; \
const char CameraParameters::FOCUS_MODE_MANUAL[] = "manual-focus"; \
const char CameraParameters::FOCUS_MODE_MULTIWINDOWAF[] = "mw_continuous-picture"; \
const char CameraParameters::FOCUS_MODE_NORMAL[] = "normal"; \
const char CameraParameters::FPS_RANGE_SYSTEM_PROPERTY_CAMCORDER[] = "hw.camcorder.fpsrange"; \
const char CameraParameters::FPS_RANGE_SYSTEM_PROPERTY_CAMERA_FRONT[] = "persist.data.front.minfps"; \
const char CameraParameters::FPS_RANGE_SYSTEM_PROPERTY_CAMERA_REAR[] = "persist.data.rear.minfps"; \
const char CameraParameters::FPS_SLOW_MOTION[] = "120"; \
const char CameraParameters::FPS_TIME_LAPSE_1P[] = "1"; \
const char CameraParameters::FPS_TIME_LAPSE_2P[] = "2"; \
const char CameraParameters::ISO_100[] = "100"; \
const char CameraParameters::ISO_1600[] = "1600"; \
const char CameraParameters::ISO_200[] = "200"; \
const char CameraParameters::ISO_3200[] = "3200"; \
const char CameraParameters::ISO_400[] = "400"; \
const char CameraParameters::ISO_800[] = "800"; \
const char CameraParameters::ISO_AUTO[] = "auto"; \
const char CameraParameters::IS_LOWLIGHT[] = "is-lowlight"; \
const char CameraParameters::KEY_BEAUTY[] = "beautyshot"; \
const char CameraParameters::KEY_BEAUTY_LEVEL[] = "beauty_level"; \
const char CameraParameters::KEY_BURST_SHOT[] = "burst-shot"; \
const char CameraParameters::KEY_BURST_SHOT_SUPPORTED[] = "burst-shot-supported"; \
const char CameraParameters::KEY_CAMERA_SWITCHING[] = "key_switching"; \
const char CameraParameters::KEY_DUAL_RECORDER[] = "dual-recorder"; \
const char CameraParameters::KEY_EXIF_DATE[] = "exif-datetime"; \
const char CameraParameters::KEY_FILM_ENABLE[] = "film_enable"; \
const char CameraParameters::KEY_FILM_TYPE[] = "film_type"; \
const char CameraParameters::KEY_HDR_MODE[] = "hdr-mode"; \
const char CameraParameters::KEY_HFR[] = "video-hfr"; \
const char CameraParameters::KEY_ISO[] = "iso"; \
const char CameraParameters::KEY_JPEG_THUMBNAIL_SIZE[] = "jpeg-thumbnail-size"; \
const char CameraParameters::KEY_LGE_CAMERA[] = "lge-camera"; \
const char CameraParameters::KEY_LG_EXPOSURE_COMPENSATION[] = "lg-ev-ctrl"; \
const char CameraParameters::KEY_LG_EXPOSURE_COMPENSATION_MAX[] = "lg-ev-ctrl-max"; \
const char CameraParameters::KEY_LG_EXPOSURE_COMPENSATION_MIN[] = "lg-ev-ctrl-min"; \
const char CameraParameters::KEY_LG_EXPOSURE_COMPENSATION_STEP[] = "lg-ev-ctrl-step"; \
const char CameraParameters::KEY_LG_WB[] = "lg-wb"; \
const char CameraParameters::KEY_MANUAL_ISO[] = "lg-iso"; \
const char CameraParameters::KEY_MANUAL_ISO_AUTO[] = "auto"; \
const char CameraParameters::KEY_MANUAL_ISO_VALUES[] = "lg-iso-values"; \
const char CameraParameters::KEY_MANUAL_MODE_RESET[] = "lg-manual-mode-reset"; \
const char CameraParameters::KEY_METERING_MODE[] = "meter-mode"; \
const char CameraParameters::KEY_MTK_CAMMODE[] = "cam-mode"; \
const char CameraParameters::KEY_QC_MAX_NUM_REQUESTED_FACES[] = "qc-max-num-requested-faces"; \
const char CameraParameters::KEY_QC_SNAPSHOT_PICTURE_FLIP[] = "snapshot-picture-flip"; \
const char CameraParameters::KEY_QC_SUPPORTED_CAMERA_FEATURES[] = "qc-camera-features"; \
const char CameraParameters::KEY_QC_VIDEO_FLIP[] = "video-flip"; \
const char CameraParameters::KEY_RAW_FORMAT[] = "dng-capture"; \
const char CameraParameters::KEY_RAW_SIZE[] = "dng-size"; \
const char CameraParameters::KEY_SHUTTER_SPEED[] = "shutter-speed"; \
const char CameraParameters::KEY_SHUTTER_SPEED_SUPPOPRTED_VALUES[] = "shutter-speed-values"; \
const char CameraParameters::KEY_SINGLE_ISP_OUTPUT_ENABLED[] = "single-isp-output-enabled"; \
const char CameraParameters::KEY_STEADY_CAM[] = "steady_video"; \
const char CameraParameters::KEY_VIDEO_HDR_MODE[] = "video-hdr"; \
const char CameraParameters::KEY_VIDEO_HDR_SUPPORTED[] = "video-hdr-supported"; \
const char CameraParameters::KEY_VIDEO_SNAPSHOT_SIZE_SUPPORTED[] = "supported-live-snapshot-sizes"; \
const char CameraParameters::KEY_VIEW_MODE[] = "view_mode"; \
const char CameraParameters::KEY_ZSL[] = "zsl"; \
const char CameraParameters::KEY_ZSL_BUFF_COUNT[] = "zsl-burst-count"; \
const char CameraParameters::LUMINANCE_CONDITION[] = "luminance-condition"; \
const char CameraParameters::LUMINANCE_CONDITION_FOR_VIDEO[] = "luminance-condition-for-video"; \
const char CameraParameters::LUMINANCE_LOW[] = "low"; \
const char CameraParameters::MANUAL_AUDIO_DIRECTION[] = "SourceTrack.zoom_beampattern"; \
const char CameraParameters::MANUAL_AUDIO_GAIN[] = "SoundFocus.gain_step"; \
const char CameraParameters::MANUAL_AUDIO_MIC_TYPE[] = "ssr-select-mic"; \
const char CameraParameters::MANUAL_AUDIO_WIND[] = "SourceTrack.enable_wnr"; \
const char CameraParameters::MANUAL_FOCUS_STEP[] = "manualfocus_step"; \
const char CameraParameters::METER_MODE_AVERAGE[] = "meter-average"; \
const char CameraParameters::METER_MODE_CENTER[] = "meter-center"; \
const char CameraParameters::METER_MODE_SPOT[] = "meter-spot"; \
const char CameraParameters::ONE_STR[] = "1"; \
const char CameraParameters::PARAMETER_SUPERZOOM[] = "superzoom"; \
const char CameraParameters::PARAMETER_VALUE_OFF[] = "off"; \
const char CameraParameters::PARAMETER_VALUE_ON[] = "on"; \
const char CameraParameters::PARAM_VIEW_MODE_CLEAN[] = "clean"; \
const char CameraParameters::PARAM_VIEW_MODE_MANUAL_CAMERA[] = "manual"; \
const char CameraParameters::PARAM_VIEW_MODE_MANUAL_VIDEO[] = "manual_video"; \
const char CameraParameters::PARAM_VIEW_MODE_NORMAL[] = "normal"; \
const char CameraParameters::PREVIEW_FORMAT_NV12[] = "nv12-venus"; \
const char CameraParameters::PREVIEW_FORMAT_YUV420[] = "yuv420sp"; \
const char CameraParameters::PREVIEW_FPS_RANGE_DEFAULT_CAMCORDER[] = "15000,30000"; \
const char CameraParameters::PREVIEW_FPS_RANGE_DEFAULT_CAMERA[] = "15000,30000"; \
const char CameraParameters::PREVIEW_FPS_RANGE_EFFECTS_CAMERA[] = "30000,30000"; \
const char CameraParameters::PREVIEW_FPS_RANGE_MMS[] = "15000,15000"; \
const char CameraParameters::SCENE_MODE_SMART_SHUTTER[] = "smartshutter"; \
const char CameraParameters::SHUTTER_SPEED_AUTO[] = "0"; \
const char CameraParameters::SIZE_FHD_1080P[] = "1920x1080"; \
const char CameraParameters::SIZE_FHD_1088P[] = "1920x1088"; \
const char CameraParameters::SIZE_HD_720P[] = "1280x720"; \
const char CameraParameters::STEADY_VALUE_OFF[] = "off"; \
const char CameraParameters::STEADY_VALUE_ON[] = "on"; \
const char CameraParameters::TIMER_0_SEC[] = "0"; \
const char CameraParameters::TIMER_10_SEC[] = "10"; \
const char CameraParameters::TIMER_2_SEC[] = "2"; \
const char CameraParameters::TIMER_3_SEC[] = "3"; \
const char CameraParameters::TWO_STR[] = "2"; \
const char CameraParameters::USE_CUR_VALUE[] = "cur_value"; \
const char CameraParameters::VALUE_NOT_FOUND[] = "not found"; \
const char CameraParameters::VALUE_VIDEO_HDR_OFF[] = "off"; \
const char CameraParameters::VALUE_VIDEO_HDR_ON[] = "on"; \
const char CameraParameters::VIDEO_3840_BY_2160[] = "3840x2160"; \
const char CameraParameters::ZERO_STR[] = "0"; \
const char CameraParameters::ZSL_OFF[] = "off"; \
const char CameraParameters::ZSL_ON[] = "on"; \

#define CAMERA_PARAMETERS_EXTRA_H \
    static const char BURST_SHOT_OFF[]; \
    static const char BURST_SHOT_ON[]; \
    static const char CAMERA_SWITCHING_VALUE_DEFAULT[]; \
    static const char CAMERA_SWITCHING_VALUE_NORMAL_TO_WIDE[]; \
    static const char CAMERA_SWITCHING_VALUE_SAME[]; \
    static const char CAMERA_SWITCHING_VALUE_WIDE_TO_NORMAL[]; \
    static const char CAMERA_WIDE_CAMERA_ZOOM_MAX[]; \
    static const char FLASH_MODE_REAR_ON[]; \
    static const char FLIP_MODE_H[]; \
    static const char FLIP_MODE_OFF[]; \
    static const char FLIP_MODE_V[]; \
    static const char FLIP_MODE_VH[]; \
    static const char FOCUS_FACE_TRACKING[]; \
    static const char FOCUS_MODE_MANUAL[]; \
    static const char FOCUS_MODE_MULTIWINDOWAF[]; \
    static const char FOCUS_MODE_NORMAL[]; \
    static const char FPS_RANGE_SYSTEM_PROPERTY_CAMCORDER[]; \
    static const char FPS_RANGE_SYSTEM_PROPERTY_CAMERA_FRONT[]; \
    static const char FPS_RANGE_SYSTEM_PROPERTY_CAMERA_REAR[]; \
    static const char FPS_SLOW_MOTION[]; \
    static const char FPS_TIME_LAPSE_1P[]; \
    static const char FPS_TIME_LAPSE_2P[]; \
    static const char ISO_100[]; \
    static const char ISO_1600[]; \
    static const char ISO_200[]; \
    static const char ISO_3200[]; \
    static const char ISO_400[]; \
    static const char ISO_800[]; \
    static const char ISO_AUTO[]; \
    static const char IS_LOWLIGHT[]; \
    static const char KEY_BEAUTY[]; \
    static const char KEY_BEAUTY_LEVEL[]; \
    static const char KEY_BURST_SHOT[]; \
    static const char KEY_BURST_SHOT_SUPPORTED[]; \
    static const char KEY_CAMERA_SWITCHING[]; \
    static const char KEY_DUAL_RECORDER[]; \
    static const char KEY_EXIF_DATE[]; \
    static const char KEY_FILM_ENABLE[]; \
    static const char KEY_FILM_TYPE[]; \
    static const char KEY_HDR_MODE[]; \
    static const char KEY_HFR[]; \
    static const char KEY_ISO[]; \
    static const char KEY_JPEG_THUMBNAIL_SIZE[]; \
    static const char KEY_LGE_CAMERA[]; \
    static const char KEY_LG_EXPOSURE_COMPENSATION[]; \
    static const char KEY_LG_EXPOSURE_COMPENSATION_MAX[]; \
    static const char KEY_LG_EXPOSURE_COMPENSATION_MIN[]; \
    static const char KEY_LG_EXPOSURE_COMPENSATION_STEP[]; \
    static const char KEY_LG_WB[]; \
    static const char KEY_MANUAL_ISO[]; \
    static const char KEY_MANUAL_ISO_AUTO[]; \
    static const char KEY_MANUAL_ISO_VALUES[]; \
    static const char KEY_MANUAL_MODE_RESET[]; \
    static const char KEY_METERING_MODE[]; \
    static const char KEY_MTK_CAMMODE[]; \
    static const char KEY_QC_MAX_NUM_REQUESTED_FACES[]; \
    static const char KEY_QC_SNAPSHOT_PICTURE_FLIP[]; \
    static const char KEY_QC_SUPPORTED_CAMERA_FEATURES[]; \
    static const char KEY_QC_VIDEO_FLIP[]; \
    static const char KEY_RAW_FORMAT[]; \
    static const char KEY_RAW_SIZE[]; \
    static const char KEY_SHUTTER_SPEED[]; \
    static const char KEY_SHUTTER_SPEED_SUPPOPRTED_VALUES[]; \
    static const char KEY_SINGLE_ISP_OUTPUT_ENABLED[]; \
    static const char KEY_STEADY_CAM[]; \
    static const char KEY_VIDEO_HDR_MODE[]; \
    static const char KEY_VIDEO_HDR_SUPPORTED[]; \
    static const char KEY_VIDEO_SNAPSHOT_SIZE_SUPPORTED[]; \
    static const char KEY_VIEW_MODE[]; \
    static const char KEY_ZSL[]; \
    static const char KEY_ZSL_BUFF_COUNT[]; \
    static const char LUMINANCE_CONDITION[]; \
    static const char LUMINANCE_CONDITION_FOR_VIDEO[]; \
    static const char LUMINANCE_LOW[]; \
    static const char MANUAL_AUDIO_DIRECTION[]; \
    static const char MANUAL_AUDIO_GAIN[]; \
    static const char MANUAL_AUDIO_MIC_TYPE[]; \
    static const char MANUAL_AUDIO_WIND[]; \
    static const char MANUAL_FOCUS_STEP[]; \
    static const char METER_MODE_AVERAGE[]; \
    static const char METER_MODE_CENTER[]; \
    static const char METER_MODE_SPOT[]; \
    static const char ONE_STR[]; \
    static const char PARAMETER_SUPERZOOM[]; \
    static const char PARAMETER_VALUE_OFF[]; \
    static const char PARAMETER_VALUE_ON[]; \
    static const char PARAM_VIEW_MODE_CLEAN[]; \
    static const char PARAM_VIEW_MODE_MANUAL_CAMERA[]; \
    static const char PARAM_VIEW_MODE_MANUAL_VIDEO[]; \
    static const char PARAM_VIEW_MODE_NORMAL[]; \
    static const char PREVIEW_FORMAT_NV12[]; \
    static const char PREVIEW_FORMAT_YUV420[]; \
    static const char PREVIEW_FPS_RANGE_DEFAULT_CAMCORDER[]; \
    static const char PREVIEW_FPS_RANGE_DEFAULT_CAMERA[]; \
    static const char PREVIEW_FPS_RANGE_EFFECTS_CAMERA[]; \
    static const char PREVIEW_FPS_RANGE_MMS[]; \
    static const char SCENE_MODE_SMART_SHUTTER[]; \
    static const char SHUTTER_SPEED_AUTO[]; \
    static const char SIZE_FHD_1080P[]; \
    static const char SIZE_FHD_1088P[]; \
    static const char SIZE_HD_720P[]; \
    static const char STEADY_VALUE_OFF[]; \
    static const char STEADY_VALUE_ON[]; \
    static const char TIMER_0_SEC[]; \
    static const char TIMER_10_SEC[]; \
    static const char TIMER_2_SEC[]; \
    static const char TIMER_3_SEC[]; \
    static const char TWO_STR[]; \
    static const char USE_CUR_VALUE[]; \
    static const char VALUE_NOT_FOUND[]; \
    static const char VALUE_VIDEO_HDR_OFF[]; \
    static const char VALUE_VIDEO_HDR_ON[]; \
    static const char VIDEO_3840_BY_2160[]; \
    static const char ZERO_STR[]; \
    static const char ZSL_OFF[]; \
    static const char ZSL_ON[]; \

/* TODO: Add int compatibility
#define CAMERA_PARAMETERS_EXTRA_C \
const int CameraParameters::FOCUS_DISTANCE_FAR_INDEX[] = 2; \
const int CameraParameters::FOCUS_DISTANCE_NEAR_INDEX[] = 0; \
const int CameraParameters::FOCUS_DISTANCE_OPTIMAL_INDEX[] = 1; \
const int CameraParameters::MAX_CONTINUOUS_SHOT[] = 6; \
const int CameraParameters::MAX_TIMECATCHSHOT[] = 5; \
const int CameraParameters::PREVIEW_FPS_MAX_INDEX[] = 1; \
const int CameraParameters::PREVIEW_FPS_MIN_INDEX[] = 0; \
const int CameraParameters::PREVIEW_FPS_RANGE_MAX[] = 30000; \
const int CameraParameters::PREVIEW_FPS_RANGE_MIN[] = 15000; \

#define CAMERA_PARAMETERS_EXTRA_H \
    static const int FOCUS_DISTANCE_FAR_INDEX[]; \
    static const int FOCUS_DISTANCE_NEAR_INDEX[]; \
    static const int FOCUS_DISTANCE_OPTIMAL_INDEX[]; \
    static const int MAX_CONTINUOUS_SHOT[]; \
    static const int MAX_TIMECATCHSHOT[]; \
    static const int PREVIEW_FPS_MAX_INDEX[]; \
    static const int PREVIEW_FPS_MIN_INDEX[]; \
    static const int PREVIEW_FPS_RANGE_MAX[]; \
    static const int PREVIEW_FPS_RANGE_MIN[]; \ */
