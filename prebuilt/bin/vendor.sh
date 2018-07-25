#!/sbin/sh

# /sbin/sh runs out of TWRP.. use it.

PROGS="readlink sgdisk awk sed tail dc grep resize2fs e2fsck mke2fs"
RC=0
# Check for my needed programs
for PROG in ${PROGS} ; do
   if [ ! -x "/sbin/${PROG}" ] ; then
      echo "Missing: /sbin/${PROG}"
      RC=9
   fi
done
#
# all prebuilt
RC=0
if [ ${RC} -ne 0 ] ; then
   echo "Aborting.."
   exit 9
fi

BUSYBOX="/tmp/install/bin/busybox"

# Get botdevice.. don't assume /dev/block/sda
DISK=`${BUSYBOX} readlink /dev/block/bootdevice/by-name/userdata | ${BUSYBOX} sed -e's/[0-9]//g'`

# Check for /vendor existance
VENDOR=`/tmp/install/bin/sgdisk --pretend --print ${DISK} | ${BUSYBOX} grep -c vendor`

if [ ${VENDOR} -ge 1 ] ; then
# Got it, we're done...
   exit 0
fi

# Missing... need to create it..
${BUSYBOX} echo "/vendor missing"
#
# Get next partition...
LAST=`/tmp/install/bin/sgdisk --pretend --print ${DISK} | ${BUSYBOX} tail -1 | ${BUSYBOX} awk '{printf $1}'`
NEXT=`${BUSYBOX} echo "${LAST} 1 + p" | ${BUSYBOX} dc`
NUMPARTS=`/tmp/install/bin/sgdisk --pretend --print ${DISK} | ${BUSYBOX} grep 'holds up to' | ${BUSYBOX} awk '{printf $6}'`

# Check if we need to expand the parition table
RESIZETABLE=""
if [ ${NEXT} -gt ${NUMPARTS} ] ; then
   RESIZETABLE=" --resize-table=${NEXT}"
fi

# Get /system partion #, start, ending, code
SYSPARTNUM=`/tmp/install/bin/sgdisk --pretend --print ${DISK} | ${BUSYBOX} grep system | ${BUSYBOX} awk '{printf $1}'`
SYSSTART=`/tmp/install/bin/sgdisk --pretend --print ${DISK} | ${BUSYBOX} grep system | ${BUSYBOX} awk '{printf $2}'`
SYSEND=`/tmp/install/bin/sgdisk --pretend --print ${DISK} | ${BUSYBOX} grep system | ${BUSYBOX} awk '{printf $3}'`
SYSCODE=`/tmp/install/bin/sgdisk --pretend --print ${DISK} | ${BUSYBOX} grep system | ${BUSYBOX} awk '{printf $6}'`

# Get sector size
SECSIZE=`/tmp/install/bin/sgdisk --pretend --print ${DISK} | ${BUSYBOX} grep 'sector size' | ${BUSYBOX} awk '{printf $4}'`

## Resize part..
echo /tmp/install/bin/e2fsck /dev/block/bootdevice/by-name/system

# 512 = 512Mg..
VENDORSIZE=`${BUSYBOX} echo "512 1024 * 1024 * ${SECSIZE} / p" | ${BUSYBOX} dc`

NEWEND=`${BUSYBOX} echo "${SYSEND} ${VENDORSIZE} - p" | ${BUSYBOX} dc`
VENDORSTART=`${BUSYBOX} echo "${NEWEND} 1 + p" | ${BUSYBOX} dc`

NEWSYSSIZE=`${BUSYBOX} echo "${NEWEND} ${SYSSTART} - 1 + p" | ${BUSYBOX} dc`
MINSYSSIZE=`/tmp/install/bin/resize2fs -P /dev/block/bootdevice/by-name/system 2>/dev/null | ${BUSYBOX} grep minimum | ${BUSYBOX} awk '{printf $7}'`

# Check if /system will shrink to small
if [ ${NEWSYSSIZE} -lt 0 ] ; then
   echo "ERROR: /system will be smaller than 0."
   exit 9
fi
if [ ${NEWSYSSIZE} -lt ${MINSYSSIZE} ] ; then
   echo "ERROR: /system will be smaller than the minimum allowed."
   exit 9
fi

# Resize /system, this will preserve the data and shrink it.
${BUSYBOX} echo "*********Resize /system to ${NEWSYSSIZE} = ${NEWEND} - ${SYSSTART} + 1 (inclusize) = ${NEWSYSSIZE}"

### TO REALLY DO THIS, REMOVE THE echo ###
echo /tmp/install/bin/e2fsck -f /dev/block/bootdevice/by-name/system
### TO REALLY DO THIS, REMOVE THE echo ###
echo /tmp/install/bin/resize2fs /dev/block/bootdevice/by-name/system ${NEWSYSSIZE}

# Only echo's for now... --pretend will NOT do it..
### TO REALLY DO THIS, REMOVE THE --pretend ###
/tmp/install/bin/sgdisk --pretend ${RESIZETABLE} --delete=${SYSPARTNUM} --new=${SYSPARTNUM}:${SYSSTART}:${NEWEND} --change-name=${SYSPARTNUM}:system --new=${NEXT}:${VENDORSTART}:${SYSEND} --change-name=${NEXT}:vendor --print ${DISK}

echo /tmp/install/bin/sgdisk --pretend ${RESIZETABLE} --delete=${SYSPARTNUM} --new=${SYSPARTNUM}:${SYSSTART}:${NEWEND} --change-name=${SYSPARTNUM}:system --new=${NEXT}:${VENDORSTART}:${SYSEND} --change-name=${NEXT}:vendor --print ${DISK}

## REALLY DO THIS ##
echo /tmp/install/bin/mke2fs -t ext4 ${DISK}${NEXT}
