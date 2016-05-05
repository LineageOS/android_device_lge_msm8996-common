integer max_count=10
backup_folder=/data/ramoops
count_file=$backup_folder/next_count

if /system/bin/ls /sys/fs/pstore/console-ramoops ; then
    if /system/bin/ls $count_file ; then
        integer count=`/system/bin/cat $count_file`
        count=$count+0
        case $count in
            "" ) count=0
        esac
    else
        count=0
    fi
    echo [[[[ Written $backup_folder/ramoops$count $max_count ]]]]
    /system/bin/cat /sys/fs/pstore/console-ramoops > $backup_folder/ramoops$count
    /system/bin/cat /proc/cmdline >> $backup_folder/ramoops$count
    /system/bin/cat /proc/cmdline >> $backup_folder/cmdline$count
    # reason is att permission certification
    /system/bin/chmod -h 664 $backup_folder/ramoops$count
    /system/bin/chmod -h 664 $backup_folder/cmdline$count
    echo update_time_state >> $backup_folder/ramoops$count
    echo update_time_state >> $backup_folder/cmdline$count
    count=$count+1
    if (($count>=$max_count)) ; then
        count=0
        echo restart
    fi
    echo $count > $count_file
    /system/bin/chmod -h 664 $count_file
fi
