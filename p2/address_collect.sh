#!/bin/sh
ifconfig eth0 | grep "inet " | awk '{printf $2" "$4}'