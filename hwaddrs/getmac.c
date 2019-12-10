/*
 * Copyright (C) 2011-2015 The CyanogenMod Project
 * Copyright (C) 2017 The LineageOS Project
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

#include <ctype.h>
#include <cutils/properties.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Validates the contents of the given file
int checkAddr(char* filepath, int key) {
	chmod(filepath,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	char charbuf[17];
	int i, notallzeroes = 0, ret = 0;
	int checkfd = open(filepath, O_RDONLY);

	if (checkfd < 0) return 0; // doesn't exist/error

	do {
		if (key == 1) {
			if (read(checkfd, charbuf, 14) != 14) break;
			if (strncmp(charbuf, "cur_etheraddr=", 14) != 0) break;
		}

		if (read(checkfd, charbuf, 17) != 17) break;
		for (i = 0; i < 17; i++) {
			if (i % 3 != 2) {
				if (!isxdigit(charbuf[i])) break;
				if (charbuf[i] != '0') notallzeroes = 1;
			} else if (charbuf[i] != ':') break;
		}
		ret = notallzeroes;
	} while (0);

	close(checkfd);
	return ret;
}

// Writes a file using an address from the misc partition
// Generates a random address if the one read contains only zeroes
void writeAddr(char* filepath, int offset, int key) {
	uint8_t macbytes[6];
	char macbuf[19];
	int i, macnums = 0;
	int miscfd = open("/dev/block/bootdevice/by-name/misc", O_RDONLY);
	int writefd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

	lseek(miscfd, offset, SEEK_SET);

	for (i = 0; i < 6; i++) {
		read(miscfd, &macbytes[i], 1);
		macnums |= macbytes[i];
	}

	close(miscfd);

	if (macnums == 0) {
		char product_name[PROPERTY_VALUE_MAX];
		property_get("ro.product.name", product_name, "");

		if (strstr(product_name, "elsa")) {
			macbytes[0] = (uint8_t) 208; // d0
			macbytes[1] = (uint8_t) 19;  // 13
			macbytes[2] = (uint8_t) 253; // fd
		} else if (strstr(product_name, "lucye")
				|| strstr(product_name, "h1")) {
			macbytes[0] = (uint8_t) 168; // a8
			macbytes[1] = (uint8_t) 184; // b8
			macbytes[2] = (uint8_t) 110; // 6e
		} else {
			// Last two bits of the first octet are special
			macbytes[0] = ((uint8_t) rand() % 256) << 2;
			macbytes[1] = (uint8_t) rand() % 256;
			macbytes[2] = (uint8_t) rand() % 256;
		}
		macbytes[3] = (uint8_t) rand() % 256;
		macbytes[4] = (uint8_t) rand() % 256;
		macbytes[5] = (uint8_t) rand() % 256;
	}

	if (key == 1) write(writefd, "cur_etheraddr=", 14);
	sprintf(macbuf, "%02x:%02x:%02x:%02x:%02x:%02x\n",
			macbytes[0], macbytes[1], macbytes[2], macbytes[3], macbytes[4], macbytes[5]);
	write(writefd, &macbuf, 18);
	close(writefd);
	chmod(filepath,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
}

// Simple file copy
void copyAddr(char* source, char* dest) {
	char buffer;
	int sourcefd = open(source, O_RDONLY);
	int destfd = open(dest, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

	if (sourcefd < 0 || destfd < 0) return; // doesn't exist/error

	while (read(sourcefd, &buffer, 1) != 0) {
		write(destfd, &buffer, 1);
	}

	close(sourcefd);
	close(destfd);
	chmod(dest,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
}

int main() {
	char *datamiscpath, *persistpath;
	srand(time(NULL));

	datamiscpath = "/data/misc/wifi/config";
	persistpath = "/persist/.macaddr";
	if (checkAddr(datamiscpath, 1) == 0) {
		if (checkAddr(persistpath, 1) == 0) {
			writeAddr(persistpath, 0x6000, 1);
		}
		copyAddr(persistpath, datamiscpath);
	}

	datamiscpath = "/data/misc/bluetooth/bdaddr";
	persistpath = "/persist/.baddr";
	if (checkAddr(datamiscpath, 0) == 0) {
		if (checkAddr(persistpath, 0) == 0) {
			writeAddr(persistpath, 0x8000, 0);
		}
		copyAddr(persistpath, datamiscpath);
	}

	return 0;
}
