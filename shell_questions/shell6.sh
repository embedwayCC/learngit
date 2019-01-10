#!/bin/bash

#question 6

i=0
while (( i < 10 ))
do
	touch test${i}.txt
	let i++
done
num=`find ./ -name "*.txt" -size 0 | wc -l`
find ./ -name "*.txt" -size 0 | xargs rm -rf
echo "has removed ${num} files!"
exit 0
