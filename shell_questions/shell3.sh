#!/bin/bash

#question 3

begin=1
end=5
if [ -z $1 ]
then
	echo "please input file!"
	exit -1
fi
line=`cat $1 | wc -l`
#echo ${line}
num=`echo "${line}/5+1" | bc`
#echo ${num}
a=1
while ((${a} <= ${num}))
do
	#echo "${begin},${end}"
	cat $1 | sed -n "${begin},${end} p" > newfile${a}.txt
	let begin=begin+5
	let end=end+5
	let a++
done
exit 0
