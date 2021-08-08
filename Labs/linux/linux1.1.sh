#!/bin/bash

script_name=$0
count=$1
folder=$2
file_number=$3
folder_name=0

echo "Скрипт $script_name запущен"
cd $folder

while [ "$folder_name" -le "$(($count - 1))" ]
do
	cd "folder$folder_name"
	rm "file$file_number"
	folder_name=$(($folder_name + 1))
	cd ..
done

exit 0