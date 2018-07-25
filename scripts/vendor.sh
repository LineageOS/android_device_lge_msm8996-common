#!/sbin/sh
#
# Copyright (C) 2018 The LineageOS Project
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
# /sbin/sh runs out of TWRP.. use it.

# Needed programs, check for each one
# Check first for /tmp static copy
# Fall back to TWRP installed copies
#
PROGS="sgdisk toybox resize2fs e2fsck mke2fs"
ABORT=0
#
# Find sgdisk
if [ -x "${SGDISK}" ] ; then
   SGDISK="${SGDISK}"
elif [ -x "/sbin/sgdisk" ] ; then
   SGDISK="/sbin/sgdisk"
else
   echo "Missing sgdisk"
   ABORT=1
fi
#
# Find toybox or busybox
if [ -x "/tmp/toybox" ] ; then
   BOX="/tmp/toybox"
elif [ -x "/sbin/busybox" ] ; then
   BOX="/sbin/busybox"
else
   echo "Missing toybox or busybox"
   ABORT=1
fi
#
# Find resize2fs
if [ -x "/tmp/resize2fs" ] ; then
   RESIZEFS="/tmp/resize2fs"
elif [ -x "/sbin/resize2fs" ] ; then
   RESIZEFS="/sbin/resize2fs"
else
   echo "Missing resize2fs"
   ABORT=1
fi
#
# Find e2fsck
if [ -x "/tmp/e2fsck" ] ; then
   FSCK="/tmp/e2fsck"
elif [ -x "/sbin/e2fsck" ] ; then
   FSCK="/sbin/e2fsck"
else
   echo "Missing e2fsck"
   ABORT=1
fi
#
# Find mke2fs
if [ -x "/tmp/mke2fs" ] ; then
   MKFS="/tmp/mke2fs"
elif [ -x "/sbin/mke2fs" ] ; then
   MKFS="/sbin/mke2fs"
else
   echo "Missing mke2fs"
   ABORT=1
fi

if [ "${ABORT}" = "1" ] ; then
   echo "Aborting.."
   exit 7
fi

#
# Get bootdevice.. don't assume /dev/block/sda
DISK=`${BOX} readlink /dev/block/bootdevice/by-name/system | ${BOX} sed -e's/[0-9]//g'`

#
# Check for /vendor existence
VENDOR=`${SGDISK} --pretend --print ${DISK} | ${BOX} grep -c vendor`

if [ ${VENDOR} -ge 1 ] ; then
   # If vendor does not have a ext4 filesystem, mke2fs it then
   if [ `${BOX} blkid /dev/block/bootdevice/by-name/vendor | ${BOX} egrep -c ext4` -eq 0 ] ; then
      ${MKFS} -t ext4 /dev/block/bootdevice/by-name/vendor
   fi
# Got it, we're done...
   exit 0
fi

# Save area.
if [ -d /data/local/tmp ] ; then
   SAVEDIR=/data/local/tmp
fi
if [ -d /data/media/0 ] ; then
   SAVEDIR=/data/media/0
fi
if [ -d /external_sd/Android ] ; then
   SAVEDIR=/external_sd
fi
#
# Save a backup of gpt
${SGDISK} --pretend --backup=${SAVEDIR}/pre-vendor-gpt.bin ${DISK}

# Missing... need to create it..
${BOX} echo "/vendor missing"
#
# Get next partition...
LAST=`${SGDISK} --pretend --print ${DISK} | ${BOX} tail -1 | ${BOX} tr -s ' ' | ${BOX} cut -d' ' -f2`
NEXT=`${BOX} expr ${LAST} + 1`
NUMPARTS=`${SGDISK} --pretend --print ${DISK} | ${BOX} grep 'holds up to' | ${BOX} tr -s ' ' | ${BOX} cut -d' ' -f6`

#
# Check if we need to expand the partition table
RESIZETABLE=""
if [ ${NEXT} -gt ${NUMPARTS} ] ; then
   RESIZETABLE=" --resize-table=${NEXT}"
fi

#
# Get /system partition #, start, ending, code
SYSPARTNUM=`${SGDISK} --pretend --print ${DISK} | ${BOX} grep system | ${BOX} tr -s ' ' | ${BOX} cut -d' ' -f2`
SYSSTART=`${SGDISK} --pretend --print ${DISK} | ${BOX} grep system | ${BOX} tr -s ' ' | ${BOX} cut -d' ' -f3`
SYSEND=`${SGDISK} --pretend --print ${DISK} | ${BOX} grep system | ${BOX} tr -s ' ' | ${BOX} cut -d' ' -f4`
SYSCODE=`${SGDISK} --pretend --print ${DISK} | ${BOX} grep system | ${BOX} tr -s ' ' | ${BOX} cut -d' ' -f7`

#
# Get sector size
SECSIZE=`${SGDISK} --pretend --print ${DISK} | ${BOX} grep 'sector size' | ${BOX} tr -s ' ' | ${BOX} cut -d' ' -f4`

#
# Sanity check
if [ "${SYSPARTNUM}" = "" -o "${SYSSTART}" = "" -o "${SYSEND}" = "" -o "${SYSCODE}" = "" -o "${SECSIZE}" = "" ] ; then
   ${BOX} echo "Failure:"
   ${BOX} echo "SYSPARTNUM=${SYSPARTNUM}"
   ${BOX} echo "SYSSTART=${SYSSTART}"
   ${BOX} echo "SYSEND=${SYSEND}"
   ${BOX} echo "SYSCODE=${SYSCODE}"
   ${BOX} echo "SECSIZE=${SECSIZE}"
   exit 9
fi

#
# 512 = 512mb
VENDORSIZE=`${BOX} expr 512 \* 1024 \* 1024 / ${SECSIZE}`

NEWEND=`${BOX} expr ${SYSEND} - ${VENDORSIZE}`
VENDORSTART=`${BOX} expr ${NEWEND} + 1`

NEWSYSSIZE=`${BOX} expr ${NEWEND} - ${SYSSTART} + 1`
MINSYSSIZE=`${RESIZEFS} -P /dev/block/bootdevice/by-name/system 2>/dev/null | ${BOX} grep minimum | ${BOX} tr -s ' ' | ${BOX} cut -d' ' -f7`

#
# Check if /system will shrink to small
if [ ${NEWSYSSIZE} -le 0 ] ; then
   ${BOX} echo "ERROR: /system will be 0."
   exit 9
fi
if [ ${NEWSYSSIZE} -lt ${MINSYSSIZE} ] ; then
   ${BOX} echo "ERROR: /system will be smaller than the minimum allowed."
   ${BOX} echo "NEWSYSSIZE=${NEWSYSSIZE}"
   ${BOX} echo "      -lt"
   ${BOX} echo "MINSYSSIZE=${MINSYSSIZE}"
   exit 9
fi

#
# Sanity checks
if [ "${NEWSYSSIZE}" = "" -o "${NEWEND}" = "" -o "${NEWSYSSIZE}" = "" ] ; then
   ${BOX} echo "Failure:"
   ${BOX} echo "NEWSYSSIZE=${NEWSYSSIZE}"
   ${BOX} echo "NEWEND=${NEWEND}"
   ${BOX} echo "NEWSYSSIZE=${NEWSYSSIZE}"
   exit 9
fi

#
# Resize /system, this will preserve the data and shrink it.
${BOX} echo "*********Resize /system to ${NEWSYSSIZE} = ${NEWEND} - ${SYSSTART} + 1 (inclusize) = ${NEWSYSSIZE}"

${FSCK} -y -f /dev/block/bootdevice/by-name/system
${RESIZEFS} /dev/block/bootdevice/by-name/system ${NEWSYSSIZE}

${SGDISK} ${RESIZETABLE} --delete=${SYSPARTNUM} --new=${SYSPARTNUM}:${SYSSTART}:${NEWEND} --change-name=${SYSPARTNUM}:system --new=${NEXT}:${VENDORSTART}:${SYSEND} --change-name=${NEXT}:vendor --print ${DISK} > /tmp/sg.out 2>&1

${BOX} echo ${SGDISK} --pretend ${RESIZETABLE} --delete=${SYSPARTNUM} --new=${SYSPARTNUM}:${SYSSTART}:${NEWEND} --change-name=${SYSPARTNUM}:system --new=${NEXT}:${VENDORSTART}:${SYSEND} --change-name=${NEXT}:vendor --print ${DISK}

${BOX} cat /tmp/sg.out

#
# Save a copy of how to undo this
${BOX} echo "To revert /vendor back: " > ${SAVEDIR}/vendor.recover.txt
${BOX} echo "**Wipe ext4 file system: " >> ${SAVEDIR}/vendor.recover.txt
${BOX} echo "dd if=/dev/zero of=/dev/block/bootdevice/by-name/vendor bs=512 count=32 conv=notrunc" >> ${SAVEDIR}/vendor.recover.txt
${BOX} echo "** Recover parition table: " >> ${SAVEDIR}/vendor.recover.txt
${BOX} echo "sgdisk --delete=${SYSPARTNUM} --delete=${NEXT} --new=${SYSPARTNUM}:${SYSSTART}:${SYSEND} --change-name=${SYSPARTNUM}:system --print ${DISK}" >> ${SAVEDIR}/vendor.recover.txt
${BOX} echo "**reboot recovery**, then resize2fs /system back to normal size:" >> ${SAVEDIR}/vendor.recover.txt
${BOX} echo "resize2fs /dev/block/bootdevice/by-name/system" >> ${SAVEDIR}/vendor.recover.txt

${BOX} cat ${SAVEDIR}/vendor.recover.txt

sleep 2
reboot recovery