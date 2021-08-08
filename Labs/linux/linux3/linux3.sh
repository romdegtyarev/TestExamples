#!/bin/bash

script_name=$0

echo "Скрипт $script_name запущен"

git status
flag=0

while [ "$flag" -le 8 ]
do
	echo "1 Добавить файлы"
	echo "2 Удалить файлы"
	echo "3 Сделать комит"
	echo "4 Редактировать ветки"
	echo "5 Загрузить на удаленный репозиторий"
	echo "6 Загрузить с удаленного репозитория"
	echo "7 Выход"
	read flag1

	case $flag1 in
		1)
		echo "Введите то, что будет включено в коммит"
		read git_add
		git add $git_add
		git status
		;;

		2)
		echo "Введите то, что будет удалено"
		read git_rm
		git rm $git_rm
		git status
		;;

		3)
		echo "Укажите комментарий"
		read comment
		git commit -m «$comment»
		;;

		4)
		echo "1 Создать новую ветку"
		echo "2 Удалить ветку"
		echo "3 Переименовать текущую ветку"
		echo "4 Перейти на ветку"
		echo "5 Объединить ветки"
		read flag2

		case $flag2 in
			1)
			echo "Введите имя ветки"
			read new_branch
			git branch $new_branch
			;;

			2)
			echo "Введите имя ветки"
			read new_branch
			git branch -D $new_branch
			;;

			3)
			echo "Введите новое имя ветки"
			read new_branch
			git branch -m $new_branch
			;;

			4)
			echo "Введите имя ветки"
			read new_branch
			git check-out $new_branch
			;;

			5)
			echo "Введите имя ветки"
			read new_branch
			git merge $new_branch
			;;

		esac
		;;

		5)
		git push
		;;

		6)
		echo "Введите имя репозитория"
		read user
		git clone $user
		;;

		7)
		exit 0
		;;

	esac

done

exit 0