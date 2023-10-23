#!/bin/bash

script_name=$0
count=$1
folder=$2
file=$3
path=$4
folder_name=0
folderin_name=0
file_name=0

echo "Скрипт $script_name запущен"
cd "$path"

while [ "$folder_name" -le "$(($count - 1))" ]
do
	mkdir "$folder$folder_name"
	cd "$folder$folder_name"
	while [ "$folderin_name" -le "$count" ]
	do
		mkdir "$folder$folderin_name"
		cd "$folder$folderin_name"
		while [ "$file_name" -le "$count" ]
		do
			touch "$file$file_name"
			file_name=$(($file_name + 1))
		done
		file_name=0
		cd ..
		folderin_name=$(($folderin_name + 1))
	done
	folderin_name=0
	cd ..
	folder_name=$(($folder_name + 1))
done

exit 0