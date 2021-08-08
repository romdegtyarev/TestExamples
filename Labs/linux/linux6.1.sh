#!/bin/bash

script_name=$0
nfs=0
ip=0
mask=0
name=0
date=0

echo "Скрипт $script_name запущен"

nfs=$(df | grep -c nfs)
ip=$(ifconfig | grep inet | grep -v inet6 | grep -v 127.0.0.1 | cut -d: -f2 | awk '{printf $1"\n"}')
mask=$(ifconfig | grep inet | grep -v inet6 | grep -v 127.0.0.1 | cut -d: -f4)
name=$(hostname)
date=$(date)

if [[ "$nfs" -eq 0 ]]
	then
		sudo mount 192.168.1.5:/home/rom/Eltex/nfs /mnt/
		echo "Папка присоединена"
		cd /mnt/
		sudo touch log
		echo "$name $ip $mask $nfs $date" >> log
else
	echo "Папка уже присоединена"
fi
exit 0