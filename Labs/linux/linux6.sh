#!/bin/bash

script_name=$0
nfs=0
ip=0
mask=0

echo "Скрипт $script_name запущен"

nfs=$(rpcinfo -p | grep -c nfs)
ip=$(ifconfig | grep inet | grep -v inet6 | grep -v 127.0.0.1 | cut -d: -f2 | awk '{printf $1"\n"}')
mask=$(ifconfig | grep inet | grep -v inet6 | grep -v 127.0.0.1 | cut -d: -f4)

if [[ "$nfs" -eq 0 ]]
	then
		echo "/home/rom/Eltex/nfs $ip/$mask(rw,no_root_squash,async,subtree_check)" | sudo tee -a /etc/exports
		sudo /etc/init.d/nfs-kernel-server restart
		echo "Доступ по NFS открыт"
else
	echo "Доступ по NFS уже открыт"
fi

exit 0