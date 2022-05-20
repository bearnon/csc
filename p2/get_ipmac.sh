#!/bin/sh
ip_mask=`ip -o -f inet addr show | awk '/scope global/{print $4}' | head -n1`
[ -n "$ip_mask" ] && nmap --unprivileged "$ip_mask" >/dev/null && sleep 1 && arp | tail -n +2 | awk '!/incomplete/{print $1,$3}'
