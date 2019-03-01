//
// Copyright 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "bluetooth_address.h"

#include <cutils/properties.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils/Log.h>

namespace {
constexpr char kNvPath[] = "/persist/.baddr";
constexpr size_t kNvPathSize = 17;
}

namespace android {
namespace hardware {
namespace bluetooth {
namespace V1_0 {
namespace implementation {

void BluetoothAddress::bytes_to_string(const uint8_t* addr, char* addr_str) {
  sprintf(addr_str, "%02x:%02x:%02x:%02x:%02x:%02x", addr[0], addr[1], addr[2],
          addr[3], addr[4], addr[5]);
}

bool BluetoothAddress::string_to_bytes(const char* addr_str, uint8_t* addr) {
//  if (addr_str == NULL) return false;
  if (addr_str == NULL) {
     ALOGD("x86cpu: %s, NULL", __func__);
     return false;
  }

//  if (strnlen(addr_str, kStringLength) != kStringLength) return false;
  if (strnlen(addr_str, kStringLength) != kStringLength) {
     ALOGD("x86cpu: %s, != %zu", __func__,  kStringLength);
     return false;
  }

  unsigned char trailing_char = '\0';

  return (sscanf(addr_str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx%1c",
                 &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5],
                 &trailing_char) == kBytes);
}

bool BluetoothAddress::get_local_address(uint8_t* local_addr) {

   int i, macnums = 0;
   char macbuf[19];
   int offset = 0x8000;

   ALOGD("x86cpu: Entering %s", __func__);
   ALOGD("x86cpu: kStringLength %zu", kStringLength);
   ALOGD("x86cpu: kNvPathSize %zu", kNvPathSize);

  chmod(kNvPath,,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

// check persist first
  ALOGD("x86cpu: %s, opening: %s", __func__,  kNvPath);
  int addr_fd = open(kNvPath, O_RDONLY);
  ALOGD("x86cpu: addr_fd = %d", addr_fd);
  if (addr_fd != -1) {
    char address[kNvPathSize + 1] = {0};
    ALOGD("x86cpu: opened  %s ok", kNvPath);

    int bytes_read = read(addr_fd, address, kNvPathSize);
    ALOGD("x86cpu: bytes_read = %d", bytes_read);

    if (bytes_read == -1) {
      ALOGE("%s: Error reading address from %s: %s", __func__, kNvPath,
            strerror(errno));
    }
    close(addr_fd);
    //address[kNvPathSize] = '\0';

    // Swap into local_addr
    string_to_bytes(address, local_addr);

    ALOGD("x86cpu: string_to_bytes true: %s",  address);
    return true;
  }

// persist failed, pull from misc parititon
  ALOGD("x86cpu: reading misc");
  int miscfd = open("/dev/block/bootdevice/by-name/misc", O_RDONLY);
  int post=lseek(miscfd, offset, SEEK_SET);
  int errnum = errno;
  ALOGD("x86cpu: seeked to: %d",post);
  ALOGD("x86cpu: seek error: %d",errnum);

  for (i = 0; i < 6; i++) {
     read(miscfd, &local_addr[i], 1);
     ALOGD("x86cpu: read 1 from misc: %02x",local_addr[i]);
     macnums |= local_addr[i];
  }
  close(miscfd);
  if (macnums > 0) {
     bytes_to_string(local_addr, macbuf);
     ALOGD("x86cpu: misc true: %s", macbuf);
     return true;
  }

// misc partition is blank, generate random
  if (macnums == 0) {
     ALOGD("x86cpu: misc, failed, random");
     char product_name[PROPERTY_VALUE_MAX];
     property_get("ro.product.name", product_name, "");

     ALOGD("x86cpu: misc product_name: %s", product_name);

     if (strstr(product_name, "elsa")) {
        local_addr[0] = (uint8_t) 208; // d0
        local_addr[1] = (uint8_t) 19;  // 13
        local_addr[2] = (uint8_t) 253; // fd
     } else if (strstr(product_name, "lucye")
           || strstr(product_name, "h1")) {
        local_addr[0] = (uint8_t) 168; // a8
        local_addr[1] = (uint8_t) 184; // b8
        local_addr[2] = (uint8_t) 110; // 6e
     } else {
  // Last two bits of the first octet are special
        local_addr[0] = ((uint8_t) rand() % 256) << 2;
        local_addr[1] = (uint8_t) rand() % 256;
        local_addr[2] = (uint8_t) rand() % 256;
     }
     local_addr[3] = (uint8_t) rand() % 256;
     local_addr[4] = (uint8_t) rand() % 256;
     local_addr[5] = (uint8_t) rand() % 256;

     int writefd = open(kNvPath, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
     bytes_to_string(local_addr, macbuf);
     write(writefd, &macbuf, kNvPathSize + 1);
     close(writefd);
     ALOGD("x86cpu: random true: %s", macbuf);

     return true;

  }

  return false;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace bluetooth
}  // namespace hardware
}  // namespace android
