#!/bin/bash

#question 11

read -p "please input a number(int):" num
i=1
j=`echo "sqrt(${num})" | bc`
#echo $j
while (( i <= j ))
do
	k=`echo "${num}%${i}" | bc`
	#echo $k
	if (( k == 0))
	then
		m=`echo "${num}/${i}" | bc`
		if (( ${i} != ${m} ))
		then
			echo "${i} ${m}"
		else
			echo "${i}"
		fi	
	fi
	let i++
done
