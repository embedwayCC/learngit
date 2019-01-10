#!/bin/bash

#question 4

if [ ! -d "/home/chencheng/learngit/shell_questions/backup/" ]
then
	mkdir /home/chencheng/learngit/shell_questions/backup/
fi
sudo tar -cvf /home/chencheng/learngit/shell_questions/backup/etc-$(date +%F).tar /etc
find ./learngit/shell_questions/backup/ -mtime +7 | xargs rm -rf
echo "shell4 crontab is running" >> /home/chencheng/learngit/shell_questions/shell4.txt
#crontab -e
#30 13 * * * /home/chencheng/learngit/shell_questions/shell4.sh
exit 0
