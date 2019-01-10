#!/bin/bash

#question 8

num=`find ./ -perm 777 | wc -l`
echo "has found ${num} dangerous files, filenames have wrote into dangerous.txt"
find ./ -perm 777 > dangerous.txt
if (( ${num} > 0 ))
then
	echo "now change 777 to 754"
	find ./ -perm 777 | xargs chmod 754
fi
exit 0 
