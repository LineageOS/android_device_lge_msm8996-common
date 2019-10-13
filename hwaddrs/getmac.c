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

#include <errno.h>
#include <log/log.h>
#include <ctype.h>
#include <cutils/properties.h>
#include <fcntl.h>
#include <selinux/selinux.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


/* the C preprocessor can be a **** to work with */
#define CONTEXT_PERSIST_STR	_EXPAND(CONTEXT_PERSIST)
#define CONTEXT_WIFI_STR	_EXPAND(CONTEXT_WIFI)
#define CONTEXT_BLUETOOTH_STR	_EXPAND(CONTEXT_BLUETOOTH)

#define _EXPAND(str) __EXPAND(str)
#define __EXPAND(str) #str


static const char TAG[] = "hwaddrs";


// Validates the contents of the given file
int checkAddr(const char *const filepath, const char *const prefix,
const int mode, const char *const context)
{
	int notallzeroes = 0;
	int checkfd = open(filepath, O_RDONLY);

	do {
		char charbuf[20]; /* needs to be more than 18 characters */
		int i;
		struct stat stat;

		if (checkfd < 0) break; // doesn't exist/error

		errno = 0; /* successful system calls don't clear errno */

		if (fstat(checkfd, &stat) < 0 || !S_ISREG(stat.st_mode)) break;
		if (mode != (stat.st_mode&mode)) break;

		if (context) {
			char *tmp;
			int rc;

			if (fgetfilecon(checkfd, &tmp) < 0) break;
			/* the following is correct, but symbol is missing */
			/* rc = selinux_file_context_cmp(tmp, context); */
			/* we only care about true or false */
			rc = strcmp(tmp, context);
			freecon(tmp);

			if (rc) break;
		}

		if (prefix) {
			if (strlen(prefix) > sizeof(charbuf)) {
				__android_log_print(ANDROID_LOG_ERROR, TAG,
"Internal error, prefix \"%s\" exceeds buffer length of %d.", prefix,
sizeof(charbuf));
				close(checkfd);
				return 0;
			}

			if (read(checkfd, charbuf, strlen(prefix)) != (ssize_t)strlen(prefix)) break;
			if (memcmp(charbuf, prefix, strlen(prefix))) break;
		}

		/* there should be 18 characters, more indicates junk at end */
		if (read(checkfd, charbuf, sizeof(charbuf)) != 18) break;
		if (!isspace(charbuf[17])) break;
		for (i = 0; i < 17; i++) {
			if (i % 3 != 2) {
				if (!isxdigit(charbuf[i])) {
					notallzeroes = 0;
					break;
				}
				if (charbuf[i] != '0') notallzeroes = 1;
			} else if (charbuf[i] != ':') {
				notallzeroes = 0;
				break;
			}
		}
	} while (0);

	if (!notallzeroes && errno != ENOENT) {
		__android_log_print(ANDROID_LOG_INFO, TAG,
"Removing corrupt \"%s\" file", filepath);
		if (unlink(filepath) < 0) __android_log_print(ANDROID_LOG_ERROR,
TAG, "unlink() failed: %s", strerror(errno));
	} else __android_log_print(ANDROID_LOG_INFO, TAG,
"File \"%s\" %s", filepath, notallzeroes?"validated":"doesn't exist");

	if (checkfd >= 0) close(checkfd);

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
	int writefd = -1;
	const char *errmsg = NULL;

	/* This call is expected to return failure status if running under
	** Android's init.  In such case our permissions are limited.  Such a
	** failure isn't a problem, as such the log message is mostly
	** debugging. */
	__android_log_print(ANDROID_LOG_DEBUG, TAG,
"setfscreatecon(): Returned %s status",
setfscreatecon(CONTEXT_PERSIST_STR) >= 0?"success":"failure");

	if ((writefd = open(filepath, O_WRONLY|O_CREAT|O_EXCL, S_IRUSR)) < 0) {
		errmsg = "open() of \"%s\" failed: %s";
		goto abort;
	}

	do {
		if ((miscfd = open("/dev/block/bootdevice/by-name/misc", O_RDONLY)) < 0) {
			errmsg = "open";
			break;
		}

		if (pread(miscfd, macbytes, sizeof(macbytes), offset) != sizeof(macbytes)) {
			errmsg = "pread";
			break;
		}

		for (i = 0; i < sizeof(macbytes); ++i) macnums |= macbytes[i];
	} while(0);

	if (errmsg) __android_log_print(ANDROID_LOG_ERROR, TAG,
"%s() of misc failed: %s", errmsg, strerror(errno));

	/* close()ing if open() failed is suboptimal, but harmless */
	close(miscfd);
	miscfd = -1;

	__android_log_print(ANDROID_LOG_INFO, TAG, "Using %s for \"%s\"",
macnums?"data from misc":"random data", filepath);

	if (macnums == 0) {
		const char rerr[] = "read() of /dev/urandom failed: %2$s";
		char product_name[PROPERTY_VALUE_MAX];
		property_get("ro.product.name", product_name, "");

		if ((miscfd = open("/dev/urandom", O_RDONLY)) < 0) {
			errmsg = "open() of /dev/urandom failed: %2$s";
			goto abort;
		}

		if (strstr(product_name, "elsa")) {
			macbytes[0] = 0xD0u;
			macbytes[1] = 0x13u;
			macbytes[2] = 0xFDu;
		} else if (strstr(product_name, "lucye")
				|| strstr(product_name, "h1")) {
			macbytes[0] = 0xA8u;
			macbytes[1] = 0xB8u;
			macbytes[2] = 0x6Eu;
		} else {
			if (read(miscfd, macbytes, 3) != 3) {
				errmsg = rerr;
				goto abort;
			}
			// Last two bits of the first octet are special
			macbytes[0] <<= 2;
		}

		if (read(miscfd, macbytes+3, 3) != 3) {
			errmsg = rerr;
			goto abort;
		}

		close(miscfd);
		miscfd = -1;
	}

	if (prefix && write(writefd, prefix, strlen(prefix)) != (ssize_t)strlen(prefix)) {
		errmsg = "write() of \"%s\" failed: %s";
		goto abort;
	}
	snprintf(macbuf, sizeof(macbuf), "%02x:%02x:%02x:%02x:%02x:%02x\n",
			macbytes[0], macbytes[1], macbytes[2], macbytes[3], macbytes[4], macbytes[5]);
	if (write(writefd, &macbuf, 18) != 18) {
		errmsg = "write() of \"%s\" failed: %s";
		goto abort;
	}
	if (close(writefd) < 0) {
		errmsg = "close() of \"%s\" failed: %s";
		goto abort;
	}
	return;

abort:
	__android_log_print(ANDROID_LOG_ERROR, TAG, errmsg, filepath,
strerror(errno));

	if (miscfd >= 0) close(miscfd);
	if (writefd >= 0) {
		__android_log_print(ANDROID_LOG_INFO, TAG,
"Removing failed \"%s\" file", filepath);
		/* unlink() first so file contents may never exit FS journal */
		if (unlink(filepath) < 0) __android_log_print(ANDROID_LOG_ERROR,
TAG, "unlink() failed: %s", strerror(errno));
		close(writefd);
	}
}

// Simple file copy
void copyAddr(const char *const source, const char *const dest,
const char *const context)
{
	char buffer[128];
	ssize_t bufcnt;
	int sourcefd = open(source, O_RDONLY);
	int destfd = -1;
	const char *errmsg;

	if (sourcefd < 0) {
		errmsg = "open() of \"%3$s\" failed: %2$s";
		goto abort;
	}

	/* This call is expected to return failure status if running under
	** Android's init.  In such case our permissions are limited.  Such a
	** failure isn't a problem, as such the log message is mostly
	** debugging. */
	__android_log_print(ANDROID_LOG_DEBUG, TAG,
"setfscreatecon(): Returned %s status",
setfscreatecon(context) >= 0?"success":"failure");

	if ((destfd = open(dest, O_WRONLY|O_CREAT|O_EXCL,
S_IRUSR|S_IRGRP|S_IROTH)) < 0) {
		errmsg = "open() of \"%s\" failed: %s";
		goto abort;
	}

	while((bufcnt = read(sourcefd, buffer, sizeof(buffer))) > 0) {
		if (write(destfd, buffer, bufcnt) != bufcnt) {
			errmsg = "write() of \"%s\" failed: %s";
			goto abort;
		}
	}

	if (bufcnt < 0) {
		errmsg = "read() of \"%3$s\" failed: %2$s";
		goto abort;
	}

	close(sourcefd);
	sourcefd = -1;
	if (close(destfd) < 0) {
		errmsg = "close() of \"%s\" failed: %s";
		goto abort;
	}
	return;

abort:
	__android_log_print(ANDROID_LOG_ERROR, TAG, errmsg,
dest, strerror(errno), source);

	if (sourcefd >= 0) close(sourcefd);
	if (destfd >= 0) {
		__android_log_print(ANDROID_LOG_INFO, TAG,
"Removing failed \"%s\" file", dest);
		/* unlink() first so file contents may never exit FS journal */
		if (unlink(dest) < 0) __android_log_print(ANDROID_LOG_ERROR,
TAG, "unlink() failed: %s", strerror(errno));
		close(destfd);
	}
}


void handlemac(const char *const datamisc, const char *const persist,
int offset, const char *const prefix, const char *const context)
{
	if (!checkAddr(datamisc, prefix, S_IRUSR|S_IRGRP|S_IROTH, context)) {
		if (!checkAddr(persist, prefix, S_IRUSR, CONTEXT_PERSIST_STR))
			writeAddr(persist, offset, prefix);
		copyAddr(persist, datamisc, context);
	}
}


int main()
{
	/* we are apparently invoked with a restrictive umask */
	umask(S_IWUSR|S_IWGRP|S_IWOTH);

	handlemac("/data/misc/wifi/config", "/persist/.macaddr", 0x6000,
"cur_etheraddr=", CONTEXT_WIFI_STR);

	handlemac("/data/misc/bluetooth/bdaddr", "/persist/.baddr", 0x8000,
NULL, CONTEXT_BLUETOOTH_STR);

	return 0;
}
