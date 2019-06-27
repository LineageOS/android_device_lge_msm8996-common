/*
 * Copyright (C) 2011-2015 The CyanogenMod Project
 * Copyright (C) 2017-2019 The LineageOS Project
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

#include <log/log.h>
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


static const char TAG[]="hwaddrs";


// Validates the contents of the given file
int checkAddr(const char *const filepath, const char *const prefix)
{
	int notallzeroes = 0;
	int checkfd = open(filepath, O_RDONLY);

	do {
		char charbuf[17];
		int i;
		struct stat stat;
		const int mode=S_IRUSR|S_IRGRP|S_IROTH;

		if (checkfd < 0) break; // doesn't exist/error

		if(fstat(checkfd, &stat)<0 || !S_ISREG(stat.st_mode)) break;
		if(mode!=(stat.st_mode&mode)) break;

		if (prefix) {
			if (read(checkfd, charbuf, strlen(prefix)) != (ssize_t)strlen(prefix)) break;
			if (memcmp(charbuf, prefix, strlen(prefix))) break;
		}

		if (read(checkfd, charbuf, 17) != 17) break;
		for (i = 0; i < 17; i++) {
			if (i % 3 != 2) {
				if (!isxdigit(charbuf[i])) {
					notallzeroes=0;
					break;
				}
				if (charbuf[i] != '0') notallzeroes = 1;
			} else if (charbuf[i] != ':') {
				notallzeroes=0;
				break;
			}
		}
	} while (0);

	if(!notallzeroes && errno!=ENOENT) {
		unlink(filepath);
		__android_log_print(ANDROID_LOG_INFO, TAG,
"Removing corrupt \"%s\" file", filepath);
	} else __android_log_print(ANDROID_LOG_INFO, TAG,
"File \"%s\" %s", filepath, notallzeroes?"validated":"doesn't exist");

	if(checkfd>=0) close(checkfd);

	return notallzeroes;
}

// Writes a file using an address from the misc partition
// Generates a random address if the one read contains only zeroes
void writeAddr(const char *const filepath, int offset, const char *const prefix)
{
	uint8_t macbytes[6];
	char macbuf[19];
	unsigned int i, macnums = 0;
	int miscfd = -1;
	int writefd = open(filepath, O_WRONLY|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	const char *errmsg=NULL;

	if(writefd<0) {
		errmsg="open() of \"%s\" failed: %s";
		goto abort;
	}

	do {
		if((miscfd=open("/dev/block/bootdevice/by-name/misc", O_RDONLY))<0) {
			errmsg="open";
			break;
		}

		if(pread(miscfd, macbytes, sizeof(macbytes), offset)!=sizeof(macbytes)) {
			errmsg="pread";
			break;
		}

		for (i = 0; i < sizeof(macbytes); ++i) macnums |= macbytes[i];
	} while(0);

	if(errmsg) __android_log_print(ANDROID_LOG_ERROR, TAG,
"%s() of misc failed: %s", errmsg, strerror(errno));

	/* close()ing if open() failed is suboptimal, but harmless */
	close(miscfd);
	miscfd=-1;

	__android_log_print(ANDROID_LOG_INFO, TAG, "Using %s for \"%s\"",
macnums?"data from misc":"random data", filepath);

	if (macnums == 0) {
		uint8_t buf[3];
		const char rerr[]="read() of /dev/urandom failed: %2$s";
		char product_name[PROPERTY_VALUE_MAX];
		property_get("ro.product.name", product_name, "");

		if((miscfd=open("/dev/urandom", O_RDONLY))<0) {
			errmsg="open() of /dev/urandom failed: %2$s";
			goto abort;
		}

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
			if(read(miscfd, buf, sizeof(buf))!=sizeof(buf)) {
				errmsg=rerr;
				goto abort;
			}
			// Last two bits of the first octet are special
			macbytes[0] = buf[0] << 2;
			macbytes[1] = buf[1];
			macbytes[2] = buf[2];
		}

		if(read(miscfd, buf, sizeof(buf))!=sizeof(buf)) {
			errmsg=rerr;
			goto abort;
		}
		macbytes[3] = buf[0];
		macbytes[4] = buf[1];
		macbytes[5] = buf[2];

		close(miscfd);
		miscfd=-1;
	}

	if (prefix) if(write(writefd, prefix, strlen(prefix))!=(ssize_t)strlen(prefix)) {
		errmsg="write() of \"%s\" failed: %s";
		goto abort;
	}
	snprintf(macbuf, sizeof(macbuf), "%02x:%02x:%02x:%02x:%02x:%02x\n",
			macbytes[0], macbytes[1], macbytes[2], macbytes[3], macbytes[4], macbytes[5]);
	if(write(writefd, &macbuf, 18)!=18) {
		errmsg="write() of \"%s\" failed: %s";
		goto abort;
	}
	if(close(writefd)<0) {
		errmsg="close() of \"%s\" failed: %s";
		goto abort;
	}
	return;

abort:
	__android_log_print(ANDROID_LOG_ERROR, TAG, errmsg, filepath,
strerror(errno));

	if(miscfd>=0) close(miscfd);
	if(writefd>=0) {
		/* unlink() first so file contents may never exit FS journal */
		unlink(filepath);
		__android_log_print(ANDROID_LOG_INFO, TAG,
"Removing failed \"%s\" file", filepath);
		close(writefd);
	}
}

// Simple file copy
void copyAddr(const char *const source, const char *const dest)
{
	char buffer[128];
	ssize_t bufcnt;
	int sourcefd = open(source, O_RDONLY);
	int destfd = open(dest, O_WRONLY|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	const char *errmsg;

	if(sourcefd<0) {
		errmsg="open() of \"%3$s\" failed: %2$s";
		goto abort;
	}
	if(destfd<0) {
		errmsg="open() of \"%s\" failed: %s";
		goto abort;
	}

	while((bufcnt=read(sourcefd, buffer, sizeof(buffer))) > 0) {
		if(write(destfd, buffer, bufcnt)!=bufcnt) {
			errmsg="write() of \"%s\" failed: %s";
			goto abort;
		}
	}

	if(bufcnt<0) {
		errmsg="read() of \"%3$s\" failed: %2$s";
		goto abort;
	}

	close(sourcefd);
	sourcefd=-1;
	if(close(destfd)<0) {
		errmsg="close() of \"%s\" failed: %s";
		goto abort;
	}
	return;

abort:
	__android_log_print(ANDROID_LOG_ERROR, TAG, errmsg,
dest, strerror(errno), source);

	if(sourcefd>=0) close(sourcefd);
	if(destfd>=0) {
		/* unlink() first so file contents may never exit FS journal */
		unlink(dest);
		__android_log_print(ANDROID_LOG_INFO, TAG,
"Removing failed \"%s\" file", dest);
		close(destfd);
	}
}


void handlemac(const char *const datamisc, const char *const persist,
int offset, const char *const prefix, bool readmisc)
{
	if(readmisc) {
		if(!checkAddr(persist, prefix))
			writeAddr(persist, offset, prefix);
	} else {
		if(!checkAddr(datamisc, prefix))
			copyAddr(persist, datamisc);
	}
}


int main(int argc, char **argv)
{
	bool do_readmisc=0, do_ieee80211mac=0, do_bluetoothmac=0;

	/* we are apparently invoked with a restrictive umask */
	umask(S_IWGRP | S_IWOTH);

	if(argc==1) {
		if(!strcmp(argv[1], "readmisc"))
			do_readmisc=do_ieee80211mac=do_bluetoothmac=1;
		else if(!strcmp(argv[1], "ieee80211mac"))
			do_ieee80211mac=1;
		else if(!strcmp(argv[1], "bluetoothmac"))
			do_bluetoothmac=1;
	}

	if(!do_ieee80211mac&&!do_bluetoothmac) {
		fprintf(stderr,
"Usage: %s <readmisc|ieee80211mac|bluetoothmac>\n"
"readmisc\n"
"    Attempts to retrieves 802.11 and Bluetooth MAC addresses from \"misc\"\n"
"    area and stash them as /persist/.macaddr and /persist/.baddr.  If unable\n"
"    to find data in misc, attempt to generate random addresses.  Needs to be\n"
"    invoked with root permissions to read misc.\n"
"ieee80211mac\n"
"    Copy 802.11 MAC address to /data/misc/wifi/config, MUST be invoked as\n"
"    system:wifi to ensure correct ownership.\n"
"bluetoothmac\n"
"    Copy Bluetooth MAC address to /data/misc/bluetooth/bdaddr, MUST be invoked\n"
"    as bluetooth:bluetooth to ensure correct ownership.\n", argv[0]);
		return 1;
	}

	if(do_ieee80211mac) handlemac("/data/misc/wifi/config",
"/persist/.macaddr", 0x6000, "cur_etheraddr=", do_readmisc);

	if(do_bluetoothmac) handlemac("/data/misc/bluetooth/bdaddr",
"/persist/.baddr", 0x8000, NULL, do_readmisc);

	return 0;
}
