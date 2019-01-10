#!/bin/bash

#question 9

find /home/chencheng/ -name "*.c" -o -name "*.h" | xargs wc -l > linecount.txt
#wc -l `find /home/chencheng/ -name "*.c" -o -name "*.h"` > linecount.txt
#find /home/chencheng/ -name "*.c" -o -name "*.h" | wc -l >> linecount.txt
exit 0
