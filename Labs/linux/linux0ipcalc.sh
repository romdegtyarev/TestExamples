#!/bin/bash

script_name=$0

echo "Скрипт $script_name запущен"

ip1=$(echo $1 | cut -d. -f1 | awk '{printf $1"\n"}')
ip2=$(echo $1 | cut -d. -f2 | awk '{printf $1"\n"}')
ip3=$(echo $1 | cut -d. -f3 | awk '{printf $1"\n"}')
ip4=$(echo $1 | cut -d. -f4 | awk '{printf $1"\n"}')

ip1b=$(echo "obase=2;$ip1" | bc)
ip2b=$(echo "obase=2;$ip2" | bc)
ip3b=$(echo "obase=2;$ip3" | bc)
ip4b=$(echo "obase=2;$ip4" | bc)

mask1=$(echo $2 | cut -d. -f1 | awk '{printf $1"\n"}')
mask2=$(echo $2 | cut -d. -f2 | awk '{printf $1"\n"}')
mask3=$(echo $2 | cut -d. -f3 | awk '{printf $1"\n"}')
mask4=$(echo $2 | cut -d. -f4 | awk '{printf $1"\n"}')

mask1b=$(echo "obase=2;$mask1" | bc)
mask2b=$(echo "obase=2;$mask2" | bc)
mask3b=$(echo "obase=2;$mask2" | bc)
mask4b=$(echo "obase=2;$mask4" | bc)

echo "IP: $1"
echo "Mask: $2"
echo "obase=2;255-$mask1" | bc
exit 0