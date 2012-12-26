#!/bin/sh

echo "$1"
iptables -I INPUT -s $1 -j DROP
