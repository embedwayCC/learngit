/* ls.h */

#ifndef LS_H
#define LS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<syslog.h>
#include<stdbool.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<dirent.h>

#define PARAM_NUM 60
#define LENGTH 256
#define FILE_MAX_NUM 1000


/*****************************************************************************************
flag[0] -> -a	flag[8] -> -f		flag[16] -> -k	flag[24] -> -q	flag[32] -> -u		
flag[1] -> -A	flag[9] -> -F	flag[17] -> -l	flag[25] -> -Q	flag[33] -> -U		
flag[2] -> -b	flag[10] -> -g	flag[18] -> -L	flag[26] -> -r	flag[34] -> -v		
flag[3] -> -B	flag[11] -> -G	flag[19] -> -m	flag[27] -> -R	flag[35] -> -w		
flag[4] -> -c	flag[12] -> -h	flag[20] -> -n	flag[28] -> -s	flag[36] -> -x		
flag[5] -> -C	flag[13] -> -H	flag[21] -> -N	flag[29] -> -S	flag[37] -> -X		
flag[6] -> -d	flag[14] -> -i	flag[22] -> -o	flag[30] -> -t	flag[38] -> -1		
flag[7] -> -D	flag[15] -> -I	flag[23] -> -p	flag[31] -> -T	flag[39] -> --author
flag[40] -> --block-size=SIZE							flag[48] -> --indicator-style=WORD
flag[41] -> --color[=WHEN]								flag[49] -> --show-control-chars
flag[42] -> --file-type									flag[50] -> --quoting-style=WORD
flag[43] -> --format=WORD								flag[51] -> --sort=WORD
flag[44] -> --full-time									flag[52] -> --time=WORD
flag[45] -> --si											flag[53] -> --time-style=STYLE
flag[46] -> --dereference-command-line-symlink-to-dir	flag[54] -> --help
flag[47] -> --hide=PATTERN								flag[55] -> --version
******************************************************************************************/

extern int GetParam(int argc, char *argv[]);

extern int DoLs(char *path);

extern int HandleFormat(struct stat *buf, char *fname, char *path);

extern int LPrint(char *fname);  

extern int HandleSort(struct stat sort[], char sname[LENGTH][LENGTH], int num);

extern int FormatFlagb(char *path);

extern int FormatFlagB(char fname[]);

extern int SortFlagc(struct stat sort[], char sname[LENGTH][LENGTH], int num);

//extern int SortFlagC(char *fname);

//extern int FormatFlagD(char *fname);

//extern int FormatFlagf(char *fname);

extern int FormatFlagF(struct stat *buf);

//extern int FormatFlagg(char *fname);

//extern int FormatFlagG(char *fname);

//extern int FormatFlagH(char *fname);

extern int FormatFlagi(struct stat *buf);

//extern int FormatFlagI(char *fname);

//extern int FormatFlagk(char *fname);

//extern int FormatFlagL(char *fname);

extern int FormatFlagm(char *path);

//extern int FormatFlagn(char *fname);

//extern int FormatFlagN(char *fname);

//extern int FormatFlago(char *fname);

extern int FormatFlagp(struct stat *buf);

extern int FormatFlagq(char *path);

//extern int FormatFlagQ(char *fname);

extern int SortFlagr(struct stat sort[], char sname[LENGTH][LENGTH], int num);

//extern int FormatFlagR(char *fname);

//extern int FormatFlags(char *fname);

extern int SortFlagS(struct stat sort[], char sname[LENGTH][LENGTH], int num);

extern int SortFlagt(struct stat sort[], char sname[LENGTH][LENGTH], int num);

//extern int FormatFlagT(char *fname);

extern int SortFlagu(struct stat sort[], char sname[LENGTH][LENGTH], int num);

//extern int FormatFlagU(char *fname);

//extern int SortFlagv(char *fname);

//extern int FormatFlagw(char *fname);

//extern int FormatFlagx(char *fname);

//extern int SortFlagX(char *fname);

//extern int FormatFlag1(char *fname);
extern int SortByName(struct stat sort[], char sname[LENGTH][LENGTH], int num);

extern int PrintFileName(struct stat *buf, char *path);

#endif


