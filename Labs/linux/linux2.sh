#!/bin/bash

script_name=$0
path=$1
flag=0

echo "Скрипт $script_name Alarm запущен"
path="/usr/share/sounds/freedesktop/stereo/alarm-clock-elapsed.oga"

while [ "$flag" -le 4 ]
do
	echo "1 Добавить"
	echo "2 Удалить"
	echo "3 Редактировать"
	echo "4 Выход"

	read flag

	case $flag in
		1)
		echo "Добавить будильник:"
		echo "Введите время (часы, минуты)"
		read hour
		read min
		crontab -l > alarm
		echo "$min $hour * * * zenity --warning --text="Alarm" paplay $path #Alarm" >> alarm
		cat alarm | crontab
		echo "Будильник добавлен"
		;;

		2)
		echo "Удалить будильники:"
		crontab -l | grep Alarm > alarm
		count=0
		while read alarms;
		do
			count=$(($count + 1))
			echo "$count) $alarms";
		done < alarm
		echo "Укажите номер"
		read string
		sed -i /$string/d alarm
		cat alarm | crontab
		;;

		3)
		echo "Редактировать будильники:"
		crontab -l | grep Alarm > alarm
		count=0
		while read alarms;
		do
			count=$(($count + 1))
			echo "$count) $alarms";
		done < alarm
		echo "Укажите номер"
		read string
		sed -i /$string/d alarm
		echo "Введите время (часы, минуты)"
		read hour
		read min
		crontab -l > alarm
		echo "$min $hour * * * zenity --warning --text="Alarm" paplay $path #Alarm" >> alarm
		cat alarm | crontab
		echo "Будильник изменен"
		cat alarm | crontab
		;;

		4)
		echo "Скрипт $script_name Alarm остановлен"
		rm alarm
		exit 0
		;;
	esac
done

echo "Скрипт $script_name Alarm остановлен"
rm alarm
exit 0