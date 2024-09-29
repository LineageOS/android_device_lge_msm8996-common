#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.fixups_lib import (
    lib_fixups,
    lib_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

namespace_imports = [
    'device/lge/msm8996-common',
    'hardware/qcom-caf/msm8996',
    'vendor/lge/msm8996-common',
    'vendor/qcom/opensource/dataservices',
]

lib_fixups: lib_fixups_user_type = {
    **lib_fixups,
}

blob_fixups: blob_fixups_user_type = {
    "system/etc/permissions/qti_libpermissions.xml": blob_fixup()
        .regex_replace(r'name="android\.hidl\.manager-V1\.0-java', r'name="android.hidl.manager@1.0-java'),
    "system_ext/etc/init/dpmd.rc": blob_fixup()
        .regex_replace(r"/system/product/bin/", r"/system/system_ext/bin/"),
    (
        "system_ext/etc/permissions/com.qti.dpmframework.xml",
        "system_ext/etc/permissions/dpmapi.xml",
    ): blob_fixup()
        .regex_replace(r"/system/product/framework/", r"/system/system_ext/framework/"),
    "system_ext/lib64/libdpmframework.so": blob_fixup()
        .binary_regex_replace(b"libhidltransport.so", b"libcutils-v29.so\x00\x00\x00"),
    "vendor/bin/hw/vendor.display.color@1.0-service": blob_fixup()
        .replace_needed("libhidlbase.so", "libhidlbase-v32.so"),
    "vendor/bin/pm-service": blob_fixup()
        .add_needed("libutils-v33.so"),
    "vendor/lib/hw/camera.msm8996.so": blob_fixup()
        .binary_regex_replace(b"service.bootanim.exit", b"service.bootanim.zzzz"),
    "vendor/lib/vulkan.msm8996.so": blob_fixup()
        .binary_regex_replace(b"vulkan.msm8953.so", b"vulkan.msm8996.so"),
    "vendor/lib64/libril-qc-qmi-1.so": blob_fixup()
        .replace_needed("libhidlbase.so", "libhidlbase-v32.so"),
    "vendor/lib64/libsecureui_svcsock.so": blob_fixup()
        .replace_needed("libhidlbase.so", "libhidlbase-v32.so"),
    "vendor/lib64/libsettings.so": blob_fixup()
        .replace_needed("libprotobuf-cpp-full.so", "libprotobuf-cpp-full-v29.so"),
    "vendor/lib64/libwvhidl.so": blob_fixup()
        .replace_needed("libprotobuf-cpp-lite.so", "libprotobuf-cpp-lite-v29.so")
        .add_needed("libcrypto_shim.so"),
    "vendor/lib64/vulkan.msm8996.so": blob_fixup()
        .binary_regex_replace(b"vulkan.msm8953.so", b"vulkan.msm8996.so"),
}  # fmt: skip

module = ExtractUtilsModule(
    "msm8996-common",
    "lge",
    check_elf=False,
    blob_fixups=blob_fixups,
    lib_fixups=lib_fixups,
    namespace_imports=namespace_imports,
)

if __name__ == "__main__":
    utils = ExtractUtils.device(module)
    utils.run()
