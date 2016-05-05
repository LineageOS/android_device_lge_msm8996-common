#!/bin/sh
# loggy.sh.

date=`date +%F_%H-%M-%S`
logcat -v time -f  /data/ramoops/cm13logcat_${date}.txt
