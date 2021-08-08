#!/bin/bash

script_name=$0

echo "Скрипт $script_name запущен"

cp $1 /etc/init.d/
cd /etc/rc0.d
ln -s ../init.d/$1 ./K20$1
cd /etc/rc6.d
ln -s ../init.d/$1 ./K20$1
cd /etc/rc2.d
ln -s ../init.d/$1 ./S99$1
exit 0