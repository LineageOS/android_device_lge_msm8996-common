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

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Read plain address from misc partiton and set the Wifi and BT mac addresses accordingly */

int main() {
	int fd1, fd2;
	char macbyte;
	char macbuf[3];
	int i;

	fd1 = open("/dev/block/bootdevice/by-name/misc",O_RDONLY);
	fd2 = open("/data/misc/wifi/config",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

	write(fd2,"cur_etheraddr=",14);

	for (i = 0; i<6; i++) {
		lseek(fd1,0x6000+i,SEEK_SET);
		lseek(fd2,0,SEEK_END);
		read(fd1,&macbyte,1);
		sprintf(macbuf,"%02x",macbyte);
		write(fd2,&macbuf,2);
		if (i!=5) write(fd2,":",1);
	}

	write(fd2,"\n",1);
	close(fd2);

	fd2 = open("/data/misc/bluetooth/bdaddr",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	for (i = 0; i<6; i++) {
		lseek(fd1,0x8000+i,SEEK_SET);
		lseek(fd2,0,SEEK_END);
		read(fd1,&macbyte,1);
		sprintf(macbuf,"%02x",macbyte);
		write(fd2,&macbuf,2);
		if (i!=5) write(fd2,":",1);
	}
	close(fd2);

	close(fd1);
	return 0;
}
