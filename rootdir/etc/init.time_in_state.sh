#!/system/bin/sh

    rm /data/logger/time_in_state
    for i in 0 1 2 3
    do
        if [ -f /sys/devices/system/cpu/cpu$i/cpufreq/stats/time_in_state ]; then
            echo cpu$i >> /data/logger/time_in_state
            cat /sys/devices/system/cpu/cpu$i/cpufreq/stats/time_in_state >> /data/logger/time_in_state
        fi
    done
    chmod -h 644 /data/logger/time_in_state
