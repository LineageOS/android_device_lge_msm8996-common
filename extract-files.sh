#!/bin/bash
#set -x 

oat2dex()
{
    APK="$1"

    OAT="`dirname $APK`/arm/`basename $APK .apk`.odex"
    if [ ! -e $OAT ]; then
        return 0
    fi

    HIT=`r2 -q -c '/ dex\n035' "$OAT" 2>/dev/null | grep hit0_0 | awk '{print $1}'`
    if [ -z "$HIT" ]; then
        echo "ERROR: Can't find dex header of `basename $APK`"
        return 1
    fi

    SIZE=`r2 -e scr.color=false -q -c "px 4 @$HIT+32" $OAT 2>/dev/null | tail -n 1 | awk '{print $2 $3}' | sed -e "s/^/0x/" | rax2 -e`
    r2 -q -c "pr $SIZE @$HIT > /tmp/classes.dex" "$OAT" 2>/dev/null
    if [ $? -ne 0 ]; then
        echo "ERROR: Something went wrong in `basename $APK`"
    fi
}

function extract() {
    for FILE in `egrep -v '(^#|^$)' $1`; do
        OLDIFS=$IFS IFS=":" PARSING_ARRAY=($FILE) IFS=$OLDIFS
        FILE=`echo ${PARSING_ARRAY[0]} | sed -e "s/^-//g"`
        DEST=${PARSING_ARRAY[1]}
        if [ -z $DEST ]; then
            DEST=$FILE
        fi
        DIR=`dirname $FILE`
        if [ ! -d $2/$DIR ]; then
            mkdir -p $2/$DIR
        fi
        if [ "$SRC" = "adb" ]; then
            # Try CM target first
            eval $ADB pull /system/$DEST $2/$DEST
            # if file does not exist try OEM target
            if [ "$?" != "0" ]; then
                eval $ADB pull /system/$FILE $2/$DEST
            fi
            if [ "${FILE##*.}" = "apk" ]; then
                oat2dex /system/$FILE
            fi
        else
            cp $SRC/system/$FILE $2/$DEST
            # if file dot not exist try destination
            if [ "$?" != "0" ]
                then
                cp $SRC/system/$DEST $2/$DEST
            fi
            if [ "${FILE##*.}" = "apk" ]; then
                oat2dex /system/$FILE
            fi
        fi

        if [ -e /tmp/classes.dex ]; then
            zip -gjq $2/$FILE /tmp/classes.dex
            rm /tmp/classes.dex
        fi
    done
}

if [ $# -eq 0 ] || [ $# -eq 2 ]; then
  SRC=adb
  if [ "$1" == "-s" ]; then
      ADB="adb -s $2"
  else
      ADB="adb"
  fi
else
  if [ $# -eq 1 ]; then
    SRC=$1
  else
    echo "$0: bad number of arguments"
    echo ""
    echo "usage: $0 [PATH_TO_EXPANDED_ROM]"
    echo ""
    echo "If PATH_TO_EXPANDED_ROM is not specified, blobs will be extracted from"
    echo "the device using adb pull."
    exit 1
  fi
fi

BASE=../../../vendor/$VENDOR/g5-common/proprietary
rm -rf $BASE/*

DEVBASE=../../../vendor/$VENDOR/$DEVICE/proprietary
rm -rf $DEVBASE/*

extract ../../$VENDOR/g5-common/proprietary-files.txt $BASE
extract ../../$VENDOR/$DEVICE/proprietary-files.txt $DEVBASE

./setup-makefiles.sh
