#!/bin/bash

date1=`date +"%s.%N"`
sudo tar -zcvf etc.tar.gz /etc
date2=`date +"%s.%N"`
echo `awk -v x1="$(echo $date2 | cut -d '.' -f 1)" -v x2="$(echo $date1 | cut -d '.' -f 1)" -v y1="$[$(echo $date2 | cut -d '.' -f 2) / 1000]" -v y2="$[$(echo $date1 | cut -d '.' -f 2) /1000]" 'BEGIN{printf "RunTIme:%.6f s",(x1-x2)+(y1-y2)/1000000}'` >> abc.txt
sudo tar -jcvf etc.tar.bz2 /etc
date3=`date +"%s.%N"`
echo `awk -v x1="$(echo $date3 | cut -d '.' -f 1)" -v x2="$(echo $date2 | cut -d '.' -f 1)" -v y1="$[$(echo $date3 | cut -d '.' -f 2) / 1000]" -v y2="$[$(echo $date2 | cut -d '.' -f 2) /1000]" 'BEGIN{printf "RunTIme:%.6f s",(x1-x2)+(y1-y2)/1000000}'` >> abc.txt
exit 0
