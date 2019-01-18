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
#include<getopt.h>
#include<sys/ioctl.h>


#define PARAM_NUM		60
#define LENGTH			256
#define FILE_MAX_NUM	1000

#define ZERO			0
#define ONE				1
#define TWO				2
#define THREE			3
#define FOUR			4
#define FIVE			5
#define SIX				6
#define SEVEN			7
#define EIGHT			8
#define NINE			9
#define TEN				10
#define ELEVEN			11
#define TWELVE			12
#define THIRTEEN		13
#define FOURTEEN		14
#define FIFTEEN			15
#define SIXTEEN			16
#define SEVENTEEN		17
#define EIGHTEEN		18
#define NINETEEN		19
#define TWENTY			20
#define TWENTY_ONE		21
#define TWENTY_TWO		22
#define TWENTY_THREE	23
#define TWENTY_FOUR		24
#define TWENTY_FIVE		25
#define TWENTY_SIX		26
#define TWENTY_SEVEN	27
#define TWENTY_EIGHT	28
#define TWENTY_NINE		29
#define THIRTY			30
#define THIRTY_ONE		31
#define THIRTY_TWO		32
#define THIRTY_THREE	33
#define THIRTY_FOUR		34
#define THIRTY_FIVE		35
#define THIRTY_SIX		36
#define THIRTY_SEVEN	37
#define THIRTY_EIGHT	38
#define THIRTY_NINE		39
#define FORTY			40
#define FORTY_ONE		41
#define FORTY_TWO		42
#define FORTY_THREE		43
#define FORTY_FOUR		44
#define FORTY_FIVE		45
#define FORTY_SIX		46
#define FORTY_SEVEN		47
#define FORTY_EIGHT		48
#define FORTY_NINE		49
#define FIFTY			50
#define FIFTY_ONE		51
#define FIFTY_TWO		52
#define FIFTY_THREE		53
#define FIFTY_FOUR		54
#define FIFTY_FIVE		55

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

/* get param,if param exists,flag = 1 */
extern int GetParam(int argc, char *argv[]);

/* begin to do ls */
extern int DoLs(char *path);

/* according to flag of format,call different function */
extern int HandleFormat(struct stat *buf, char *fname, char *path);

/* print information (-l) */
extern int LPrint(char *fname);  

/* -t function */
extern int HandleSort(struct stat sort[], char sname[LENGTH][LENGTH], int num);

/* -b function */
extern int FormatFlagb(char *path);

/* -B function */
extern int FormatFlagB(char fname[]);

/* -c function */
extern int SortFlagc(struct stat sort[], char sname[LENGTH][LENGTH], int num);

/* -F function */
extern int FormatFlagF(struct stat *buf);

/* -i function */
extern int FormatFlagi(struct stat *buf);

/* -I function */
extern int FormatFlagI(char *path, char* optarg);

/* -m function */
extern int FormatFlagm(char *path);

/* -p function */
extern int FormatFlagp(struct stat *buf);

/* -q function */
extern int FormatFlagq(char *path);

/* -r function */
extern int SortFlagr(struct stat sort[], char sname[LENGTH][LENGTH], int num);

/* -S function */
extern int SortFlagS(struct stat sort[], char sname[LENGTH][LENGTH], int num);

/* -t function */
extern int SortFlagt(struct stat sort[], char sname[LENGTH][LENGTH], int num);

/* -u function */
extern int SortFlagu(struct stat sort[], char sname[LENGTH][LENGTH], int num);

/* default sort by file name */
extern int SortByName(struct stat sort[], char sname[LENGTH][LENGTH], int num);

/* print file name */
extern int PrintFileName(struct stat *buf, char *path);

/* --file-type function */
extern int FormatFlagFileType(struct stat *buf);

/* print time */
extern int PrintTime(struct stat buf);

/* --help function */
extern int PrintHelp();

/* print size */
extern int PrintSize(struct stat buf);

/* print file root */
extern int PrintRoot(struct stat buf);

/* print name, group and author */
extern int PrintName(struct stat buf);

/* get screen width */
extern int GetWidth();

/* get row and col */
extern int GetRowAndCol(int* row, int* col, char sname[LENGTH][LENGTH], int num);

#endif


