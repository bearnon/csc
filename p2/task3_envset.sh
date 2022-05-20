#!/bin/sh
sysctl -w net.ipv4.ip_forward=1
iptables -t nat -F
iptables -t nat -A PREROUTING -p tcp --dport 443 -j REDIRECT --to-ports 8443
openssl genrsa -out ca.key 4096
openssl req -new -x509 -days 1826 -key ca.key -out ca.crt -subj "/C=TW"
openssl pkcs12 -export -in ca.crt -inkey ca.key -out ca.p12 -password pass:123456
