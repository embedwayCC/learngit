#include"ls.h"

bool flags[PARAM_NUM] = {0};
char* fpath = NULL;
char* indicator_style = NULL;
char* hide = NULL;
char* sorts = NULL;
char* times = NULL;
char* block_size = NULL;
int terminalWidth, wordLenMax;

//bool flagR[PARAM_NUM] = {0};


/****************************************************************************
*Function: GetParam
*Description: get param,if param exists,flag = 1
*Input: argc: param number
           argv[]: value of every param
*Output: flag[]: record param
*Return: 0: success
             -1: exit
****************************************************************************/
int GetParam(int argc, char *argv[])
{
	syslog(LOG_DEBUG,"%s L#%d  into GetParam\n", __func__, __LINE__);
	int opt;
	int i;
	char *string = "aAbBcCdfFgGhHiI:klLmnopqQrRsStuU1";
	static struct option long_options[] =
    {  
        {"author", no_argument, NULL, THIRTY_NINE},
        {"block-size", required_argument, NULL, FORTY},
        {"file-type", no_argument, NULL, FORTY_TWO},
        {"format", required_argument, NULL, FORTY_THREE},
        {"full-time", no_argument, NULL, FORTY_FOUR},
        {"si", no_argument, NULL, FORTY_FIVE},
        {"dereference-command-line-symlink-to-dir", no_argument, NULL, FORTY_SIX},
        {"hide", required_argument, NULL, FORTY_SEVEN},
        {"indicator-style", required_argument, NULL, FORTY_EIGHT},
        {"show-control-chars", no_argument, NULL, FORTY_NINE},
        {"quoting-style", required_argument, NULL, FIFTY},
        {"sort", required_argument, NULL, FIFTY_ONE},
        {"time", required_argument, NULL, FIFTY_TWO},
        {"time-style", required_argument, NULL, FIFTY_THREE},
        {"help", no_argument, NULL, FIFTY_FOUR},
        {"version", no_argument, NULL, FIFTY_FIVE},
    };
	int option_index = 0;
	while((opt =getopt_long(argc,argv,string,long_options,&option_index))!= -1)
    {
		switch (opt) {		
			case 'a': flags[ZERO] = 1; break;
			case 'A': flags[ONE] = 1; break;
			case 'b': flags[TWO] = 1; break;
			case 'B': flags[THREE] = 1; break;
			case 'c': flags[FOUR] = 1; break;
			case 'C': flags[FIVE] = 1; break;
			case 'd': flags[SIX] = 1; break;
			case 'f': flags[EIGHT] = 1; break;
			case 'F': flags[NINE] = 1; break;
			case 'g': flags[TEN] = 1; break;
			case 'G': flags[ELEVEN] = 1; break;
			case 'h': flags[TWELVE] = 1; break;
			case 'H': flags[THIRTEEN] = 1; break;
			case 'i': flags[FOURTEEN] = 1; break;
			case 'I': flags[FIFTEEN] = 1; fpath = optarg; break;
			case 'k': flags[SIXTEEN] = 1; break;
			case 'l': flags[SEVENTEEN] = 1; break;
			case 'L': flags[EIGHTEEN] = 1; break;
			case 'm': flags[NINETEEN] = 1; break;
			case 'n': flags[TWENTY] = 1; break;
			case 'o': flags[TWENTY_TWO] = 1; break;
			case 'p': flags[TWENTY_THREE] = 1; break;
			case 'q': flags[TWENTY_FOUR] = 1; break;
			case 'Q': flags[TWENTY_FIVE] = 1; break;
			case 'r': flags[TWENTY_SIX] = 1; break;
			case 'R': flags[TWENTY_SEVEN] = 1; break;
			case 's': flags[TWENTY_EIGHT] = 1; break;
			case 'S': flags[TWENTY_NINE] = 1; break;
			case 't': flags[THIRTY] = 1; break;
			case 'u': flags[THIRTY_TWO] = 1; break;
			case 'U': flags[THIRTY_THREE] = 1; break;
			case '1': flags[THIRTY_EIGHT] = 1; break;
			case THIRTY_NINE: flags[THIRTY_NINE] = 1; break;
			case FORTY: flags[FORTY] = 1; block_size = optarg; break;
			case FORTY_TWO: flags[FORTY_TWO] = 1; break;
			//case FORTY_THREE: printf("--format=%s\n",optarg); flags[FORTY_THREE] = 1; break;
			case FORTY_FOUR: flags[FORTY_FOUR] = 1; break;
			case FORTY_FIVE: flags[FORTY_FIVE] = 1; break;
			//case FORTY_SIX: printf("--dereference-command-line-symlink-to-dir\n"); flags[FORTY_SIX] = 1; break;
			case FORTY_SEVEN:	if(flags[ZERO] == 1 || flags[ZERO] == 1)
									flags[FORTY_SEVEN] = 0;
								else
								{
									flags[FORTY_SEVEN] = 1;
									hide = optarg;
								}
								break;
			case FORTY_EIGHT: flags[FORTY_EIGHT] = 1; indicator_style = optarg; break;
			//case FORTY_NINE: printf("--show-control-chars\n"); flags[FORTY_NINE] = 1; break;
			//case FIFTY: printf("--quoting-style=%s\n",optarg); flags[FIFTY] = 1; break;
			case FIFTY_ONE: flags[FIFTY_ONE] = 1; 
							sorts= optarg; 
							if(strcmp(sorts, "none") == 0)
								flags[THIRTY_THREE] = 1;
							break;
			case FIFTY_TWO: flags[FIFTY_TWO] = 1; times = optarg; break;
			//case FIFTY_THREE: printf("--time-style=%s\n",optarg); flags[FIFTY_THREE] = 1; break;
			case FIFTY_FOUR: flags[FIFTY_FOUR] = 1; break;
			//case FIFTY_FIVE: printf("--version\n"); flags[FIFTY_FIVE] = 1; break;
			case '?': printf("Unknown option: %s\n", argv[optind - 1]); break;
			default : printf("Step into default\n"); break;
		}
    }
	GetWidth();
	if(flags[FIFTY_FOUR]) //--help
		PrintHelp();
	else
	{
		if(argc == optind)
		{
			DoLs(".");
		}
		else  
		{
			for(i = optind; i < argc; i++)
				DoLs(argv[i]);
		}
	}
	return 0;
}

/****************************************************************************
*Function: DoLs
*Description: begin to do ls
*Input: path: current path
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int DoLs(char *path)
{
	syslog(LOG_DEBUG,"%s L#%d  into DoLs\n", __func__, __LINE__);
	struct stat buf, statbuf;
	struct stat sort[FILE_MAX_NUM];
	DIR *dir;
	struct dirent *pd;
	char fname[LENGTH];
	char sname[LENGTH][LENGTH];
	int num, i, row, col, j, k;
	int wordMax[300];
	
	if(lstat(path, &buf) < 0)
		return -1;

	if(flags[SIX]) //-d
	{
		if(!flags[THREE] || (flags[THREE] && !FormatFlagB(path))) //-B
		{
			if(!flags[FIFTEEN] || (flags[FIFTEEN] && !FormatFlagI(path, fpath))) //-I
				HandleFormat(&buf, NULL, path);
		}
		printf("\n");
		return 0;
	}
	if(S_ISDIR(buf.st_mode)) //is directory?
	{
		syslog(LOG_DEBUG,"%s L#%d  file is dir\n", __func__, __LINE__);
		dir = opendir(path);
		if(flags[TWENTY_SEVEN]) //-R
		{
			syslog(LOG_DEBUG,"%s L#%d  do -R\n", __func__, __LINE__);
			printf("%s:\n", path);
			printf("-------------------------\n");
			flags[TWENTY_SEVEN] = 0;
			DoLs(path);
			while((pd = readdir(dir)) != NULL)
			{
				memset(fname, 0, LENGTH);
				strcpy(fname, path);
				strcat(fname, "/");
				strcat(fname, pd->d_name);
				lstat(fname, &statbuf);
				if(S_ISDIR(statbuf.st_mode) && ((flags[ZERO] && ((strcmp(pd->d_name,".") != 0) && (strcmp(pd->d_name,"..") != 0))) || (pd->d_name[0] != '.')))
				{
					flags[TWENTY_SEVEN] = 1;
					//printf("%s:\n", fname);  
					//printf("-------------------------\n");  
					DoLs(fname);
				} 
				else if(S_ISDIR(statbuf.st_mode) && (flags[ONE] && ((strcmp(pd->d_name,".") != 0) && (strcmp(pd->d_name,"..") != 0)))) //-A
				{
					flags[TWENTY_SEVEN] = 1;
					DoLs(fname);
				} 
			}
		}
		else
		{
			syslog(LOG_DEBUG,"%s L#%d  read dir, store info[] and fname[] to print\n", __func__, __LINE__);
			num = 0;
			while((pd = readdir(dir)) != NULL)
			{
				if((flags[ZERO] && (pd->d_name[0] == '.')) || (pd->d_name[0] != '.') || (flags[8] && (pd->d_name[0] == '.'))) //-a,-f
				{
					memset(fname, 0, LENGTH);
					strcpy(fname, path);
					strcat(fname, "/");
					strcat(fname, pd->d_name);
					lstat(fname, &sort[num]);
					strcpy(sname[num],pd->d_name);
					num++;  
				}
				else if(flags[ONE] && ((strcmp(pd->d_name,".") != 0) && (strcmp(pd->d_name,"..") != 0))) //-A
				{
					memset(fname, 0, LENGTH);
					strcpy(fname, path);
					strcat(fname, "/");
					strcat(fname, pd->d_name);
					lstat(fname, &sort[num]);
					strcpy(sname[num],pd->d_name);
					num++;  
				}
			}

			if(!flags[EIGHT] && !flags[THIRTY_THREE]) //-f,-U
				SortByName(sort, sname, num);
			HandleSort(sort, sname, num);
			GetRowAndCol(&row, &col, sname, num);
			//printf("row col:--->%d %d\n",row,col);

			//for(i = 0; i < num; i += col)
			//{
				//for(j = 0; j < col; j++)
			//	{
				//	if((i + j) < num)
					//{
					//	if(!flags[THREE] || (flags[THREE] && !FormatFlagB(sname[i+j]))) //-B
					//	{
						//	if(!flags[FIFTEEN] || (flags[FIFTEEN] && !FormatFlagI(sname[i+j], fpath))) //-I
						//		HandleFormat(&sort[i+j], sname[i+j] ,path);
							/*if(!flags[SEVENTEEN] && !flags[TEN] && !flags[TWENTY_TWO] && !flags[FORTY_FOUR] && !flags[THIRTY_EIGHT]) //no -l,-g,-o,--full-time,-1
							{
								if(strlen(sname[i+j])<wordLenMax)
								{
									for(k=strlen(sname[i+j]);k<wordLenMax;k++)
										printf(" ");
								}
							}*/
						//}
					//}
				//}
				//printf("\n");
			//}

			for( i=0; i<num; i+=row ){
       			k = i/row;
        		for(j = i; j < (i+row); j++){
            		wordMax[k] = wordMax[k] > strlen(sname[j]) ? wordMax[k] : strlen(sname[j]);
        		}
    		}
			if(!flags[SEVENTEEN] && !flags[TEN] && !flags[TWENTY_TWO] && !flags[FORTY_FOUR] && !flags[THIRTY_EIGHT]) //no -l,-g,-o,--full-time,-1
			{
			    for( i=0;i<row;i++ )
				{
       				j = i;
       				while( j<num ){
						if(!flags[THREE] || (flags[THREE] && !FormatFlagB(sname[j]))) //-B
						{
							if(!flags[FIFTEEN] || (flags[FIFTEEN] && !FormatFlagI(sname[j], fpath))) //-I
							{
									HandleFormat(&sort[j], sname[j] ,path);
									if(row!=1)
									{
										for(k = strlen(sname[j]) ; k <wordMax[j/row] ;k++) 
            								printf(" ");
									}
							}
						}
       					j += row;
        			}
					if(i <= row - 1)
						printf("\n");
    			}
			}
			else
			{
				j = 0;
				while(j < num){
					if(!flags[THREE] || (flags[THREE] && !FormatFlagB(sname[j]))) //-B
					{
						if(!flags[FIFTEEN] || (flags[FIFTEEN] && !FormatFlagI(sname[j], fpath))) //-I
							HandleFormat(&sort[j], sname[j] ,path);
					}
					j++;
				}
			}
			//printf("\n");
		}
		closedir(dir);
		return 0;
	}
	else
	{
		if(!flags[THREE] || (flags[THREE] && !FormatFlagB(path))) //-B
		{
			if(!flags[FIFTEEN] || (flags[FIFTEEN] && !FormatFlagI(path, fpath))) //-I
				HandleFormat(&buf, NULL, path);
		}
		printf("\n");
		return 0;
	}
}

/****************************************************************************
*Function: HandleFormat
*Description: according to flag of format,call different function
*Input: buf: the information needing to be handled
           fname: file name
           path: path
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int HandleFormat(struct stat *buf, char *fname, char *path)
{
	syslog(LOG_DEBUG,"%s L#%d  into HandleFormat\n", __func__, __LINE__);
	char name[LENGTH];
	if(flags[TWO]) //-b
		FormatFlagb(fname);
	if(flags[NINETEEN]) //-m
		FormatFlagm(fname);
	if(flags[TWENTY_FOUR]) //-q
		FormatFlagq(fname);
	if(flags[FOURTEEN]) //-i
		FormatFlagi(buf);
	if(flags[TWENTY_THREE]) //-p
		FormatFlagp(buf);
	if(flags[NINE]) //-F
		FormatFlagF(buf);
	if(flags[FORTY_TWO]) //--file-type
		FormatFlagFileType(buf);
	if(flags[FORTY_EIGHT]) //--indicator-style
	{
		if(strcmp(indicator_style, "slash") == 0) //-p
			FormatFlagp(buf);
		else if(strcmp(indicator_style, "file-type") == 0) //--file-type
			FormatFlagFileType(buf);
		else if(strcmp(indicator_style, "classify") == 0) //-F
			FormatFlagF(buf);
	}
		
	if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FORTY_FOUR]) //-l,-g,-o,--full-time
	{
		//syslog(LOG_DEBUG,"%s L#%d  do -l or -g\n", __func__, __LINE__);
		if(fname == NULL)
		{
			if(flags[TWENTY_EIGHT]) //-s
				printf("%5ld ",buf->st_blocks);
			LPrint(path);
			PrintFileName(buf, path);
		}
		else if(!flags[FORTY_SEVEN] || (flags[FORTY_SEVEN] && (strcmp(fname, hide) != 0))) //--hide
		{
			if(flags[TWENTY_EIGHT]) //-s
				printf("%5ld ",buf->st_blocks);
			memset(name, 0, LENGTH);
			strcpy(name, path);
			strcat(name, "/");
			strcat(name, fname);
			LPrint(name);
			PrintFileName(buf, fname);
		}
		return 0;
	}
	else
	{
		if(fname == NULL)
		{
			if(flags[TWENTY_EIGHT]) //-s
				printf("%5ld ",buf->st_blocks);
			PrintFileName(buf, path);
		}
		else if(!flags[FORTY_SEVEN] || (flags[FORTY_SEVEN] && (strcmp(fname, hide) != 0))) //--hide
		{
			if(flags[TWENTY_EIGHT]) //-s
				printf("%5ld ",buf->st_blocks);
			PrintFileName(buf, fname);
		}
		return 0;
	}
	//return 0;
}

/****************************************************************************
*Function: LPrint
*Description: print information (-l)
*Input: fname: the information needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int LPrint(char* fname)
{
	syslog(LOG_DEBUG,"%s L#%d  into LPrint\n", __func__, __LINE__);
	struct stat buf;
	char link[LENGTH];  

	lstat(fname, &buf);

	PrintRoot(buf);
	
	printf(" %3ld",buf.st_nlink);

	PrintName(buf);
	PrintSize(buf);
	PrintTime(buf);
         
	if(S_ISLNK(buf.st_mode) && flags[EIGHTEEN]) //-L
	{  
		printf(" -> ");  
		readlink(fname,link,100);  
		printf("%s",link);  
	}  
	printf("\t"); 

	return 0;
}

/****************************************************************************
*Function: HandleSort
*Description: according to flag of sort,call different function
*Input: sort: array of the information which have been read
           sname: array of file name
           num: file number
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int HandleSort(struct stat sort[], char sname[LENGTH][LENGTH], int num)
{
	syslog(LOG_DEBUG,"%s L#%d  into HandleSort\n", __func__, __LINE__);
	if(flags[FIFTY_ONE]) //--sort
	{
		if(strcmp(sorts, "size") == 0)
			SortFlagS(sort, sname, num);
		else if(strcmp(sorts, "time") == 0)
			SortFlagt(sort, sname, num);
	}
	if(flags[FIFTY_TWO]) //--time
	{
		if(strcmp(times, "atime") == 0 || strcmp(times, "access") == 0)
			SortFlagu(sort, sname, num);
		else if(strcmp(times, "ctime") == 0 || strcmp(times, "status") == 0)
			SortFlagc(sort, sname, num);
	}
	if(flags[THIRTY]) //-t
		SortFlagt(sort, sname, num);
	if(flags[FOUR]) //-c
		SortFlagc(sort, sname, num);
	if(flags[TWENTY_SIX]) //-r
		SortFlagr(sort, sname, num);
	if(flags[TWENTY_NINE]) //-S
		SortFlagS(sort, sname, num);
	if(flags[THIRTY_TWO]) //-u
		SortFlagu(sort, sname, num);
	return 0;
}

/****************************************************************************
*Function: FormatFlagF
*Description: -F
*Input: buf: the information needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int FormatFlagF(struct stat *buf)
{
	syslog(LOG_DEBUG,"%s L#%d  into FormatFlagF\n", __func__, __LINE__);
	if(S_ISSOCK(buf->st_mode) && !flags[TWENTY_THREE])
		printf("=");
	else if(S_ISLNK(buf->st_mode) && !flags[TWENTY_THREE])
		printf("@");
	else if(S_ISFIFO(buf->st_mode) && !flags[TWENTY_THREE])
		printf("|");
	else if(S_ISDIR(buf->st_mode) && !flags[TWENTY_THREE])
		printf("/");
	else if(buf->st_mode & S_IXUSR && !flags[TWENTY_THREE])
		printf("*");
	return 0;
}

/****************************************************************************
*Function: FormatFlagp
*Description: -p
*Input: buf: the information needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int FormatFlagp(struct stat *buf)
{
	syslog(LOG_DEBUG,"%s L#%d  into FormatFlagp\n", __func__, __LINE__);

	if(S_ISDIR(buf->st_mode))
		printf("/");
	return 0;
}

/****************************************************************************
*Function: FormatFlagi
*Description: -i
*Input: buf: the information needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int FormatFlagi(struct stat *buf)
{
	syslog(LOG_DEBUG,"%s L#%d  into FormatFlagi\n", __func__, __LINE__);
	printf("%ld ",buf->st_ino);
	return 0;
}

/****************************************************************************
*Function: SortFlagc
*Description: -c
*Input: sort: the array of the information needing to be printed
           sname: the array of the file name needing to be printed
           num: file number
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int SortFlagc(struct stat sort[], char sname[LENGTH][LENGTH], int num)
{
	syslog(LOG_DEBUG,"%s L#%d  into SortFlagc\n", __func__, __LINE__);
	struct stat tmps;
	char tmpn[LENGTH];
	int i, j;

	for(i = 0; i < num - 1; i++)
	{
		for(j = i + 1; j < num; j++)
		{
			if(sort[i].st_ctime < sort[j].st_ctime)
			{
				tmps = sort[i];
				sort[i] = sort[j];
				sort[j] = tmps;

				strcpy(tmpn, sname[i]);
				strcpy(sname[i], sname[j]);
				strcpy(sname[j], tmpn);
			}
		}
	}
	return 0;
}

/****************************************************************************
*Function: SortFlagr
*Description: -r
*Input: sort: the array of the information needing to be printed
           sname: the array of the file name needing to be printed
           num: file number
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int SortFlagr(struct stat sort[], char sname[LENGTH][LENGTH], int num)
{
	syslog(LOG_DEBUG,"%s L#%d  into SortFlagr\n", __func__, __LINE__);
	struct stat tmps;
	char tmpn[LENGTH];
	int i, j;

	for(i = 0, j = num - 1;i < j; i++, j--)
	{
		tmps = sort[i];
		sort[i] = sort[j];
		sort[j] = tmps;

		strcpy(tmpn, sname[i]);
		strcpy(sname[i], sname[j]);
		strcpy(sname[j], tmpn);
	}			
	return 0;
}

/****************************************************************************
*Function: SortFlagS
*Description: -S
*Input: sort: the array of the information needing to be printed
           sname: the array of the file name needing to be printed
           num: file number
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int SortFlagS(struct stat sort[], char sname[LENGTH][LENGTH], int num)
{
	syslog(LOG_DEBUG,"%s L#%d  into SortFlagS\n", __func__, __LINE__);
	struct stat tmps;
	char tmpn[LENGTH];
	int i, j;
	
	for(i = 0; i < num - 1; i++)
	{
		for(j = i + 1; j < num; j++)
		{
			if(sort[i].st_size < sort[j].st_size)
			{
				tmps = sort[i];
				sort[i] = sort[j];
				sort[j] = tmps;

				strcpy(tmpn, sname[i]);
				strcpy(sname[i], sname[j]);
				strcpy(sname[j], tmpn);
			}
		}
	}
	return 0;
}

/****************************************************************************
*Function: SortFlagt
*Description: -t
*Input: sort: the array of the information needing to be printed
           sname: the array of the file name needing to be printed
           num: file number
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int SortFlagt(struct stat sort[], char sname[LENGTH][LENGTH], int num)
{
	syslog(LOG_DEBUG,"%s L#%d  into SortFlagt\n", __func__, __LINE__);
	struct stat tmps;
	char tmpn[LENGTH];
	int i, j;
	
	for(i = 0; i < num - 1; i++)
	{
		for(j = i + 1; j < num; j++)
		{
			if(sort[i].st_mtime < sort[j].st_mtime)
			{
				tmps = sort[i];
				sort[i] = sort[j];
				sort[j] = tmps;

				strcpy(tmpn, sname[i]);
				strcpy(sname[i], sname[j]);
				strcpy(sname[j], tmpn);
			}
		}
	}
	return 0;
}

/****************************************************************************
*Function: SortFlagu
*Description: -u
*Input: sort: the array of the information needing to be printed
           sname: the array of the file name needing to be printed
           num: file number
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int SortFlagu(struct stat sort[], char sname[LENGTH][LENGTH], int num)
{
	syslog(LOG_DEBUG,"%s L#%d  into SortFlagu\n", __func__, __LINE__);
	struct stat tmps;
	char tmpn[LENGTH];
	int i, j;

	for(i = 0; i < num - 1; i++)
	{
		for(j = i + 1; j < num; j++)
		{
			if(sort[i].st_atime < sort[j].st_atime)
			{
				tmps = sort[i];
				sort[i] = sort[j];
				sort[j] = tmps;
					
				strcpy(tmpn, sname[i]);
				strcpy(sname[i], sname[j]);
				strcpy(sname[j], tmpn);
			}
		}
	}
	return 0;
}

/****************************************************************************
*Function: SortByName
*Description: default sort by file name
*Input: sort: the array of the information needing to be printed
           sname: the array of the file name needing to be printed
           num: file number
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int SortByName(struct stat sort[], char sname[LENGTH][LENGTH], int num)
{
	syslog(LOG_DEBUG,"%s L#%d  into SortByName\n", __func__, __LINE__);
	struct stat tmps;
	char tmpn[LENGTH];
	int i, j;

	for(i = 0; i < num - 1; i++)
	{
		for(j = i + 1; j < num; j++)
		{
			if(strcmp(sname[i],sname[j]) > 0)
			{
				tmps = sort[i];
				sort[i] = sort[j];
				sort[j] = tmps;
	
				strcpy(tmpn, sname[i]);
				strcpy(sname[i], sname[j]);
				strcpy(sname[j], tmpn);
			}
		}
	}
	return 0;
}

/****************************************************************************
*Function: FormatFlagB
*Description: -B
*Input: sname: the file name needing to be printed
*Output: nothing
*Return: 0: no ~
             1: has ~
****************************************************************************/
int FormatFlagB(char sname[])
{
	syslog(LOG_DEBUG,"%s L#%d  into FormatFlagB\n", __func__, __LINE__);
	if(sname[strlen(sname) - 1] == '~')
	{
		return 1;
	}
	return 0;
}

/****************************************************************************
*Function: PrintFileName
*Description: print file name
*Input: buf: the information needing to be printed
           path: current path
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int PrintFileName(struct stat *buf, char *path)
{
	syslog(LOG_DEBUG,"%s L#%d  into PrintFileName\n", __func__, __LINE__);
	switch(buf->st_mode & S_IFMT)
	{
		case S_IFDIR:     
			if(flags[TWENTY_FIVE]) //-Q
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[01;34m\"%s\"\n\033[0m", path);
				else
					printf("\033[01;34m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[01;34m%s\n\033[0m", path);  
				else
					printf("\033[01;34m%s  \033[0m", path);  
			}
			break;  
		case S_IFCHR:     
			if(flags[TWENTY_FIVE]) //-Q
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[40;01;33m\"%s\"\n\033[0m", path);
				else
					printf("\033[40;01;33m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[40;01;33m%s\n\033[0m", path);    
				else
					printf("\033[40;01;33m%s  \033[0m", path);    
			}  
			break;  
		case S_IFBLK:   
			if(flags[TWENTY_FIVE]) //-Q
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[40;01;33m\"%s\"\n\033[0m", path);
				else
					printf("\033[40;01;33m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[40;01;33m%s\n\033[0m", path);    
				else
					printf("\033[40;01;33m%s  \033[0m", path);    
			}   
			break;  
		case S_IFIFO: 
			if(flags[TWENTY_FIVE]) //-Q
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[40;33m\"%s\"\n\033[0m", path);
				else
					printf("\033[40;33m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[40;33m%s\n\033[0m", path);   
				else
					printf("\033[40;33m%s  \033[0m", path);  
			} 
			break;  
		case S_IFLNK:
			if(flags[TWENTY_FIVE]) //-Q
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[01;36m\"%s\"\n\033[0m", path);
				else
					printf("\033[01;36m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[01;36m%s\n\033[0m", path); 
				else
					printf("\033[01;36m%s  \033[0m", path);   
			}   
			break;  
		case S_IFSOCK: 
			if(flags[TWENTY_FIVE]) //-Q
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[01;35m\"%s\"\n\033[0m", path);
				else
					printf("\033[01;35m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
					printf("\033[01;35m%s\n\033[0m", path);  
				else
					printf("\033[01;35m%s  \033[0m", path);     
			}    
			break;  
		default:
			if(buf->st_mode & S_IXUSR || buf->st_mode & S_IXGRP || buf->st_mode & S_IXOTH)
			{			
				if(flags[TWENTY_FIVE]) //-Q
				{
					if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
						printf("\033[01;32m\"%s\"\n\033[0m", path);
					else
						printf("\033[01;32m\"%s\"  \033[0m", path);
				}
				else
				{
					if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
						printf("\033[01;32m%s\n\033[0m", path); 
					else
						printf("\033[01;32m%s  \033[0m", path); 
				} 
			}
			else
			{
				if(flags[TWENTY_FIVE]) //-Q
				{
					if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
						printf("\"%s\"\n", path);
					else
						printf("\"%s\"  ", path);
				}
				else
				{
					if(flags[SEVENTEEN] || flags[TEN] || flags[TWENTY_TWO] || flags[FIVE] || flags[FORTY_FOUR] || flags[THIRTY_EIGHT]) //-l,-g,-o,-C,-1
						printf("%s\n", path); 
					else
						printf("%s  ", path); 
				} 
			}
			break;
	}  
	return 0;
}

/****************************************************************************
*Function: FormatFlagb
*Description: -b
*Input: path: the file name needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int FormatFlagb(char *path)
{
	syslog(LOG_DEBUG,"%s L#%d  into FormatFlagb\n", __func__, __LINE__);
	int i;
	for(i = 0; i < strlen(path); i++)
	{
		if(!isprint(*(path+i)))
			printf("exist char that can't be printed!\n");
	}
	return 0;
}

/****************************************************************************
*Function: FormatFlagq
*Description: -q
*Input: path: the file name needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int FormatFlagq(char *path)
{
	syslog(LOG_DEBUG,"%s L#%d  into FormatFlagq\n", __func__, __LINE__);
	int i;
	for(i = 0; i < strlen(path); i++)
	{
		//printf("%c--",*(path + i));
		if(!isprint(*(path + i)))
			*(path + i) = '?';
	}
	return 0;
}

/****************************************************************************
*Function: FormatFlagm
*Description: -m
*Input: path: the file name needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int FormatFlagm(char *path)
{
	syslog(LOG_DEBUG,"%s L#%d  into FormatFlagm\n", __func__, __LINE__);
	strcat(path, ",");
	return 0;
}

/****************************************************************************
*Function: FormatFlagI
*Description: -I
*Input: path: the file name needing to be printed
           optarg: fpath
*Output: nothing
*Return: 0: not equal
             1: equal
****************************************************************************/
int FormatFlagI(char *path, char* optarg)
{
	syslog(LOG_DEBUG,"%s L#%d  into FormatFlagI\n", __func__, __LINE__);
	//printf("%s====%s-----\n",path,optarg);
	if(strcmp(path, optarg) == 0)
		return 1;
	return 0;
}

/****************************************************************************
*Function: FormatFlagFileType
*Description: --file-type
*Input: buf: the information needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int FormatFlagFileType(struct stat *buf)
{
	syslog(LOG_DEBUG,"%s L#%d  into FormatFlagFileType\n", __func__, __LINE__);
	if(S_ISSOCK(buf->st_mode) && !flags[TWENTY_THREE] && !flags[NINE])
		printf("=");
	else if(S_ISLNK(buf->st_mode) && !flags[TWENTY_THREE] && !flags[NINE])
		printf("@");
	else if(S_ISFIFO(buf->st_mode) && !flags[TWENTY_THREE] && !flags[NINE])
		printf("|");
	else if(S_ISDIR(buf->st_mode) && !flags[TWENTY_THREE] && !flags[NINE])
		printf("/");
	return 0;
}

/****************************************************************************
*Function: PrintTime
*Description: print time
*Input: buf: the information needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int PrintTime(struct stat buf)
{
	syslog(LOG_DEBUG,"%s L#%d  into PrintTime\n", __func__, __LINE__);
	struct tm *t;

	if(flags[FOUR]) //-c
	{
		t = localtime(&buf.st_ctime);
		if(flags[FORTY_FOUR]) //--full-time
		{
			printf(" %4d-%02d-%02d %02d:%02d:%02d %s"  
				,t->tm_year+1900  
				,t->tm_mon+1  
				,t->tm_mday  
				,t->tm_hour  
				,t->tm_min
				,t->tm_sec
				,t->tm_zone);
		}
		else
		{
			printf(" %4d-%02d-%02d %02d:%02d"  
				,t->tm_year+1900  
				,t->tm_mon+1  
				,t->tm_mday  
				,t->tm_hour  
				,t->tm_min);
		}
	}
	else if(flags[THIRTY_TWO]) //-u
	{
		t = localtime(&buf.st_atime);
		if(flags[FORTY_FOUR]) //--full-time
		{
			printf(" %4d-%02d-%02d %02d:%02d:%02d %s"  
				,t->tm_year+1900  
				,t->tm_mon+1  
				,t->tm_mday  
				,t->tm_hour  
				,t->tm_min
				,t->tm_sec
				,t->tm_zone);
		}
		else
		{
			printf(" %4d-%02d-%02d %02d:%02d"  
				,t->tm_year+1900  
				,t->tm_mon+1  
				,t->tm_mday  
				,t->tm_hour  
				,t->tm_min);
		}
	}
	else
	{
		t = localtime(&buf.st_mtime);
		if(flags[FORTY_FOUR]) //--full-time
		{
			printf(" %4d-%02d-%02d %02d:%02d:%02d %s"
				,t->tm_year+1900  
				,t->tm_mon+1  
				,t->tm_mday  
				,t->tm_hour  
				,t->tm_min
				,t->tm_sec
				,t->tm_zone);
		}
		else
		{
			printf(" %4d-%02d-%02d %02d:%02d"
				,t->tm_year+1900  
				,t->tm_mon+1  
				,t->tm_mday  
				,t->tm_hour  
				,t->tm_min);
		}
	}
	return 0;
}

/****************************************************************************
*Function: PrintHelp
*Description: print help
*Input: nothing
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int PrintHelp()
{
	syslog(LOG_DEBUG,"%s L#%d  into PrintHelp\n", __func__, __LINE__);
	
	printf(" \
-a		do not ignore entries starting with .\n \
-A		do not list implied . and ..\n \
-b		print C-style escapes for nongraphic characters\n \
-B		do not list implied entries ending with ~\n \
-c		with -lt: sort by, and show, ctime (time of last modification of file status information); with -l: show ctime and sort by name; otherwise: sort by ctime, newest first\n \
-C		list entries by columns\n \
-d		list directories themselves, not their contents\n \
-f		do not sort, enable -aU, disable -ls --color\n \
-F		append indicator (one of */=>@|) to entries\n \
-g		like -l, but do not list owner\n \
-G		in a long listing, don't print group names\n \
-h		with -l and/or -s, print human readable sizes (e.g., 1K 234M 2G)\n \
-H		follow symbolic links listed on the command line\n \
-i		print the index number of each file\n \
-I		do not list implied entries matching shell PATTERN\n \
-k		default to 1024-byte blocks for disk usage\n \
-l		use a long listing format\n \
-L		when showing file information for a symbolic link, show information for the file the link references rather than for the link itself\n \
-m		fill width with a comma separated list of entries\n \
-n		like -l, but list numeric user and group IDs\n \
-o		like -l, but do not list group information\n \
-p		append / indicator to directories\n \
-q		print ? instead of nongraphic characters\n \
-Q		enclose entry names in double quotes\n \
-r		reverse order while sorting\n \
-R		list subdirectories recursively\n \
-s		print the allocated size of each file, in blocks\n \
-S		sort by file size\n \
-t		sort by modification time, newest first\n \
-u		with -lt: sort by, and show, access time; with -l: show access time and sort by name; otherwise: sort by access time\n \
-U		do not sort; list entries in directory order\n \
-1		list one file per line\n \
--author			with -l, print the author of each file\n \
--block-size=SIZE		scale sizes by SIZE before printing them; e.g., '--block-size=M' prints sizes in units of 1,048,576 bytes; see SIZE format below\n \
--file-type			likewise, except do not append '*'\n \
--full-time			like -l --time-style=full-iso\n \
--si				likewise, but use powers of 1000 not 1024\n \
--hide=PATTERN			do not list implied entries matching shell PATTERN (overridden by -a or -A)\n \
--indicator-style=WORD		append indicator with style WORD to entry names:none (default), slash (-p), file-type (--file-type), classify (-F)\n \
--sort=WORD			sort by WORD instead of name: none (-U), size (-S), time (-t)\n \
--time=WORD			with -l, show time as WORD instead of default modification time: atime or access or use (-u) ctime or status (-c); also use specified time as sort key if --sort=time\n \
--help				display this help and exit\n");
	return 0;
}

/****************************************************************************
*Function: PrintSize
*Description: print size
*Input: buf: the information needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int PrintSize(struct stat buf)
{
	syslog(LOG_DEBUG,"%s L#%d  into PrintSize\n", __func__, __LINE__);

	float a;

	if(!flags[TWELVE] && !flags[THIRTEEN] && !flags[SIXTEEN] && !flags[FORTY_FIVE] && !flags[FORTY]) //no  -h,-H,-k,--si,--block-size
		printf(" %10ld",buf.st_size);
	else if(flags[TWELVE]) //-h
	{
		if(buf.st_size > 1024)
		{
			a = (float)buf.st_size / 1024;
			printf(" %10.1fK", a);
		}
		else
			printf(" %10ld",buf.st_size);
	}
	else if(flags[SIXTEEN]) //-k
	{
		printf(" %10d",(int)buf.st_size / 1024 + 1);
	}
	else if(flags[FORTY]) //--block-size
	{
		if(strcmp(block_size, "k") == 0)
		{
			if(buf.st_size != 0)
				printf(" %10dk",(int)buf.st_size / 1024 + 1);
			else
				printf(" %10ldk",buf.st_size);
		}
		else if(strcmp(block_size, "M") == 0)
		{
			if(buf.st_size != 0)
				printf(" %10dM",(int)buf.st_size / 1024/ 1024 + 1);
			else
				printf(" %10ldk",buf.st_size);
		}
		else
			printf(" %10ld",buf.st_size);
	}
	else
	{
		if(buf.st_size > 1000)
		{
			a = (float)buf.st_size / 1000;
			printf(" %10.1fK", a);
		}
		else
			printf(" %10ld",buf.st_size);
	}
	return 0;
}

/****************************************************************************
*Function: PrintRoot
*Description: print root
*Input: buf: the information needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int PrintRoot(struct stat buf)
{
	syslog(LOG_DEBUG,"%s L#%d  into PrintSize\n", __func__, __LINE__);
	int n;
	switch(buf.st_mode & S_IFMT)
	{
		case S_IFREG:     
			printf("-");  
			break;  
		case S_IFDIR:     
			printf("d");  
			break;  
		case S_IFCHR:     
			printf("c");  
			break;  
		case S_IFBLK:     
			printf("b");  
			break;  
		case S_IFIFO:     
			printf("p");  
			break;  
		case S_IFLNK:     
			printf("l");  
			break;  
		case S_IFSOCK:    
			printf("s");  
			break;  
	}  
	for(n = 8; n >= 0; n--)
	{  
		if(buf.st_mode&(1 << n))  
		{  
			switch(n % 3)  
			{  
				case 2:  
					printf("r");  
					break;  
				case 1:  
					printf("w");  
					break;  
				case 0:  
					printf("x");  
					break;  
				default:  
					break;  
			}  
		}  
		else  
		{  
			printf("-");  
		}  
	}   
	return 0;
}

/****************************************************************************
*Function: PrintName
*Description: print name, group and author
*Input: buf: the information needing to be printed
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int PrintName(struct stat buf)
{
	syslog(LOG_DEBUG,"%s L#%d  into PrintSize\n", __func__, __LINE__);
	struct passwd *pw;  
	struct group *gr; 

	if(!flags[TWENTY]) //-n
	{
		pw = getpwuid(buf.st_uid);
		if(!flags[TEN]) //-g
			printf(" %11s",pw->pw_name);
  
		gr = getgrgid(buf.st_gid);
		if(!flags[ELEVEN] && !flags[TWENTY_TWO]) //-G,-o
			printf(" %11s",gr->gr_name); 
	}
	else
	{
		if(!flags[TEN]) //-g
			printf(" %3d",buf.st_uid);
		if(!flags[ELEVEN] && !flags[TWENTY_TWO]) //-G,-o
			printf(" %3d",buf.st_gid);
	}

	if(flags[THIRTY_NINE]) //--author
	{
		pw = getpwuid(buf.st_uid);
		printf(" %11s",pw->pw_name);
	}
	return 0;
}

/****************************************************************************
*Function: GetWidth
*Description: get screen width
*Input: nothing
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int GetWidth()
{
	syslog(LOG_DEBUG,"%s L#%d  into GetWidth\n", __func__, __LINE__);
	char *tp;
	struct winsize wbuf;
	terminalWidth = 80;
	if( isatty(STDOUT_FILENO) ){  
		if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &wbuf) == -1 || wbuf.ws_col == 0){
			if( tp = getenv("COLUMNS") )
				terminalWidth = atoi( tp );
		}
		else
			terminalWidth = wbuf.ws_col;
	}
	else
		flags[THIRTY_EIGHT] = 1; //-1
	//printf("terminalWidth:--->%d\n",terminalWidth);
	return 0;
}

/****************************************************************************
*Function: GetRowAndCol
*Description: get row and col
*Input: nothing
*Output: nothing
*Return: 0: success
             -1: exit
****************************************************************************/
int GetRowAndCol(int* row, int* col, char sname[LENGTH][LENGTH], int num)
{
	syslog(LOG_DEBUG,"%s L#%d  into GetRowAndCol\n", __func__, __LINE__);
	int i, j, sum_length = 0;
	bool flag = 0;
	for(i = 0; i < num; i++)
	{
		if(i == 0) 
			wordLenMax = strlen(sname[i]);
		else 
			wordLenMax = wordLenMax > strlen(sname[i]) ? wordLenMax:strlen(sname[i]);
	}
	//printf("%d==============\n",wordLenMax);

	if(flags[FOURTEEN]) //-i
		*col = terminalWidth / (wordLenMax + TWO +NINE);
	else
		*col = terminalWidth / (wordLenMax + TWO);
	if(num % *col == 0)
		*row = num / *col;
	else
		*row = num / *col + 1;
	
	if(num % *row == 0)
		*col = num / *row;
	else
		*col = num / *row + 1;

	while(*row != 0)
	{
		for(i = 0; i < *row; i++)
		{
			for(j = 0; j < *col; j++)
			{
				if((i + j * (*row)) < num)
				{
					if(flags[FOURTEEN]) //-i
						sum_length = sum_length + strlen(sname[(i + j * (*row))]) + TWO + NINE;
					else
						sum_length = sum_length + strlen(sname[(i + j * (*row))]) + TWO;
				}
			}
			if(sum_length > terminalWidth)
			{
				flag = 1;
				break;
			}
			else
				sum_length = 0;
		}
		if(flag)
		{
			(*row)++;
			break;
		}
		else
		{
			(*row)--;
			if(*row != 0)
			{
				if(num % *row == 0)
					*col = num / *row;
				else
					*col = num / *row + 1;
			}
			else
			{
				(*row)++;
				break;
			}
		}
	}
	if(num % *row == 0)
		*col = num / *row;
	else
		*col = num / *row + 1;
	return 0;
}

