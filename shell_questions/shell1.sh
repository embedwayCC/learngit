#!/bin/bash

#question 1

upper_to_lower()
{
	read -p "please input string needed to be transformed:" string
	echo $string | tr '[:upper:]' '[:lower:]'
	num=`echo $string | sed 's/ //g' |wc -m`
	echo "$num -1" |bc
}
lower_to_upper()
{
	read -p "please input string needed to be transformed:" string
	echo $string | tr '[:lower:]' '[:upper:]'
	num=`echo $string | sed 's/ //g' |wc -m`
	echo "$num -1" |bc
}

read -p "please input command string(lower->tr into 'a'-'z', upper->tr into 'A'-'Z', exit or quit->finish program):" string
while [[ $string != "exit" && $string != "quit" ]]
do
	if [ $string = "lower" ]
	then
		upper_to_lower
		read -p "please input command string again:" string
	elif [ $string = "upper" ]
	then
		lower_to_upper
		read -p "please input command string again:" string
	else
		echo $string | tr '[:upper:]' '[:lower:]'
		num=`echo $string | sed 's/ //g' |wc -m`
		echo "$num -1" |bc
		read -p "please input command string again:" string
	fi
done
exit 0
