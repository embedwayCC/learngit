#!/bin/bash

#question 5

read -p "please input number(int),'q' for exit:" num
sum=0
while [ ${num} != 'q' ]
do
	echo "${num}*${num}" | bc
	let sum=sum+${num}*${num}
	read -p "please input number(int),'q' for exit:" num
done
echo "sum is:${sum}"
exit 0
