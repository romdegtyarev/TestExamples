#!/bin/bash

script_name=$0
grep_proc=0
flag=0

echo "Скрипт $script_name запущен"

if [[ "$1" = "-l" ]]
	then
		while [ "$flag" -le 4 ]
		do
			echo "1 Добавить"
			echo "2 Удалить"
			echo "3 Редактировать"
			echo "4 Выход"

			read flag

			case $flag in
				1)
				echo "Добавить время:"
				read min
				echo "Добавить процесс:"
				read proc
				echo "$min * * * * $script_name $proc $min #Procc" >> procc
				cat procc | crontab
				;;

				2)
				echo "Удалить:"
				crontab -l | grep "#Procc" > procc
				count=0
				while read procces;
				do
					count=$(($count + 1))
					echo "$count) $procces";
				done < procc
				echo "Укажите номер"
				read string
				sed -i /$string/d procc
				cat procc
				cat procc | crontab
				;;

				3)
				echo "Редактировать:"
				crontab -l | grep Alarm > procc
				count=0
				while read procces;
				do
					count=$(($count + 1))
					echo "$count) $procces";
				done < procc
				echo "Укажите номер"
				read string
				sed -i /$string/d procc
				echo "Введите периодичность проверки:"
				read min
				crontab -l > procc
				echo "$min * * * * $script_name #Procc" >> procc
				cat procc | crontab
				echo "Время изменено"
				cat procc | crontab
				;;

				4)
				echo "Скрипт $script_name остановлен"
				exit 0
				;;
			esac

		done

else
		crontab -l | grep "$script_name $1 $2 #Procc" > procc
		count=0
		while read procces;
		do
			count=$(($count + 1))
			echo "$count) $procces";
		done < procc
		if [[ "$count" = 0 ]]
			then
				echo "$2 * * * * $script_name $1 $2 #Procc" >> procc
				cat procc | crontab
				echo "Задание добавлено"
		else
				echo "Задание уже добавлено"
		fi
fi

rm procc
exit 0