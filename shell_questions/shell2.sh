#!/bin/bash

#question 2

if [ ! -d "/home/chencheng/HOME/" ]
then
	mkdir /home/chencheng/HOME/
else
	echo "exist"
fi
n=1
while ((${n} <= 50))
do
	cd /home/chencheng/HOME/
	mkdir user${n}
	chmod 754 user${n}
	let n++
done
exit 0
