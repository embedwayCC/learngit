#include"ls.h"

bool flags[PARAM_NUM] = {0};
char* fpath = NULL;
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
	char ch;
	int i;
    while ((ch = getopt(argc, argv, "aAbBcCdfFgGhHiI:klLmnopqQrRStuU")) != -1) {      
        switch (ch) {      
            case 'a': flags[0] = 1; break;
			case 'A': flags[1] = 1; break;
			case 'b': flags[2] = 1; break;
			case 'B': flags[3] = 1; break;
			case 'c': flags[4] = 1; break;
			case 'C': flags[5] = 1; break;
			case 'd': flags[6] = 1; break;
			case 'f': flags[8] = 1; break;
			case 'F': flags[9] = 1; break;
			case 'g': flags[10] = 1; break;
			case 'G': flags[11] = 1; break;
			case 'h': flags[12] = 1; break;
			case 'H': flags[13] = 1; break;
			case 'i': flags[14] = 1; break;
			case 'I': flags[15] = 1; fpath = optarg; break;
			case 'k': flags[16] = 1; break;
			case 'l': flags[17] = 1; break;
			case 'L': flags[18] = 1; break;
			case 'm': flags[19] = 1; break;
			case 'n': flags[20] = 1; break;
			case 'o': flags[22] = 1; break;
			case 'p': flags[23] = 1; break;
			case 'q': flags[24] = 1; break;
			case 'Q': flags[25] = 1; break;
			case 'r': flags[26] = 1; break;
			case 'R': flags[27] = 1; break;
			case 'S': flags[29] = 1; break;
			case 't': flags[30] = 1; break;
			case 'u': flags[32] = 1; break;
			case 'U': flags[33] = 1; break;
            case '?': printf("Unknown option: %c\n", (char)optopt); break;
            default : printf("Step into default\n"); break;
        }      
    }
	//for(i = 0; i < 7; i++)  
		//flagR[i] = flags[i];	
	if(argc == optind)
	{
		DoLs(".");
	}
	else  
	{
		for(i = optind; i < argc; i++)
			DoLs(argv[i]);
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
	int num, i;
	
	if(lstat(path, &buf) < 0)
		return -1;

	//strcpy(fname,path); 

	if(flags[6]) //-d
	{
		if(!flags[3] || (flags[3] && !FormatFlagB(path))) //-B
		{
			if(!flags[15] || (flags[15] && !FormatFlagI(path, fpath))) //-I
				HandleFormat(&buf, NULL, path);
		}
		/*syslog(LOG_DEBUG,"%s L#%d  do -d\n", __func__, __LINE__);
		HandleFormat(buf);
		
		if(flags[17] || flags[10] || flags[22]) //-l,-g,-o
		{
			syslog(LOG_DEBUG,"%s L#%d  do -l or -g\n", __func__, __LINE__);
			LPrint(path);
			PrintFileName(buf, path);
			return 0;
		}
		else if(flags[19]) //-m
		{
			PrintFileName(buf, path);
			return 0;
		}
		else
		{
			PrintFileName(buf, path);
			return 0;
		}*/
		printf("\n");
		return 0;
	}
	if(S_ISDIR(buf.st_mode)) //is directory?
	{
		syslog(LOG_DEBUG,"%s L#%d  file is dir\n", __func__, __LINE__);
		dir = opendir(path);
		if(flags[27]) //-R
		{
			syslog(LOG_DEBUG,"%s L#%d  do -R\n", __func__, __LINE__);
			printf("%s:\n", path);
			printf("-------------------------\n");
			flags[27] = 0;
			DoLs(path);
			while((pd = readdir(dir)) != NULL)
			{
				memset(fname, 0, LENGTH);
				strcpy(fname, path);
				strcat(fname, "/");
				strcat(fname, pd->d_name);
				lstat(fname, &statbuf);
				if(S_ISDIR(statbuf.st_mode) && ((flags[0] && ((strcmp(pd->d_name,".") != 0) && (strcmp(pd->d_name,"..") != 0))) || (pd->d_name[0] != '.')))
				{
					flags[27] = 1;
					//printf("%s:\n", fname);  
					//printf("-------------------------\n");  
					DoLs(fname);
				} 
				else if(S_ISDIR(statbuf.st_mode) && (flags[1] && ((strcmp(pd->d_name,".") != 0) && (strcmp(pd->d_name,"..") != 0)))) //-A
				{
					flags[27] = 1;
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
				if((flags[0] && (pd->d_name[0] == '.')) || (pd->d_name[0] != '.') || (flags[8] && (pd->d_name[0] == '.'))) //-a,-f
				{
					memset(fname, 0, LENGTH);
					strcpy(fname, path);
					strcat(fname, "/");
					strcat(fname, pd->d_name);
					lstat(fname, &sort[num]);
					strcpy(sname[num],pd->d_name);
					num++;  
				}
				else if(flags[1] && ((strcmp(pd->d_name,".") != 0) && (strcmp(pd->d_name,"..") != 0))) //-A
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

			if(!flags[8] && !flags[33]) //-f,-U
				SortByName(sort, sname, num);
			HandleSort(sort, sname, num);

			for(i = 0;i < num; i++)
			{
				if(!flags[3] || (flags[3] && !FormatFlagB(sname[i]))) //-B
				{
					if(!flags[15] || (flags[15] && !FormatFlagI(sname[i], fpath))) //-I
						HandleFormat(&sort[i], sname[i] ,path);
				}
				/*HandleFormat(sort[i]);

				if(!flags[3] || (flags[3] && !FormatFlagB(sname[i]))) //-B
				{
					if(flags[17] || flags[10] || flags[22]) //-l,-g,-o
					{
						memset(fname, 0, LENGTH);
						strcpy(fname, path);
						strcat(fname, "/");
						strcat(fname, sname[i]);
						LPrint(fname);
						PrintFileName(sort[i], sname[i]);
					}
					else if(flags[19]) //-m
					{
						PrintFileName(sort[i], sname[i]);
					}
					else
					{
						PrintFileName(sort[i], sname[i]);
					}
				}*/
			}
			printf("\n");
		}
		closedir(dir);
		return 0;
	}
	else
	{
		if(!flags[3] || (flags[3] && !FormatFlagB(path))) //-B
		{
			if(!flags[15] || (flags[15] && !FormatFlagI(path, fpath))) //-I
				HandleFormat(&buf, NULL, path);
		}
		/*syslog(LOG_DEBUG,"%s L#%d  file is not dir\n", __func__, __LINE__);
		HandleFormat(buf);
		if(flags[17] || flags[10] || flags[22]) //-l,-g,-o
		{
			LPrint(path);
			PrintFileName(buf, path);
			return 0;
		}
		else if(flags[19]) //-m
		{
			PrintFileName(buf, path);
			return 0;
		}
		else
		{
			PrintFileName(buf, path);
			return 0;
		}*/
		printf("\n");
		return 0;
	}
}

/****************************************************************************
*Function: HandleFormat
*Description: according to flag of format,call different function
*Input: buf:the information needing to be handled
*Output: nothing
*Return: 0: success
         -1: exit
****************************************************************************/
int HandleFormat(struct stat *buf, char *fname, char *path)
{
	/*syslog(LOG_DEBUG,"%s L#%d  into HandleFormat\n", __func__, __LINE__);
	//if(flags[2]) //-b
		//FormatFlagb(&buf);
	if(flags[14]) //-i
		FormatFlagi(buf);
	if(flags[23]) //-p
		FormatFlagp(buf);
	if(flags[9]) //-F
		FormatFlagF(buf);
	return 0;*/
	syslog(LOG_DEBUG,"%s L#%d  into HandleFormat\n", __func__, __LINE__);
	char name[LENGTH];
	if(flags[2]) //-b
		FormatFlagb(fname);
	if(flags[19]) //-m
		FormatFlagm(fname);
	if(flags[24]) //-q
		FormatFlagq(fname);
	if(flags[14]) //-i
		FormatFlagi(buf);
	if(flags[23]) //-p
		FormatFlagp(buf);
	if(flags[9]) //-F
		FormatFlagF(buf);
		
	if(flags[17] || flags[10] || flags[22]) //-l,-g,-o
	{
		//syslog(LOG_DEBUG,"%s L#%d  do -l or -g\n", __func__, __LINE__);
		if(fname == NULL)
		{
			LPrint(path);
			PrintFileName(buf, path);
		}
		else
		{
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
			PrintFileName(buf, path);
		else
			PrintFileName(buf, fname);
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
	int n;  
	char link[256];  
	struct passwd *pw;  
	struct group *gr;  
	struct tm *t;
	float a;

	lstat(fname, &buf);
  
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
	printf(" %3ld",buf.st_nlink);

	if(!flags[20]) //-n
	{
		pw = getpwuid(buf.st_uid);
		if(!flags[10]) //-g
			printf(" %11s",pw->pw_name);
  
		gr = getgrgid(buf.st_gid);
		if(!flags[11] && !flags[22]) //-G,-o
			printf(" %11s",gr->gr_name); 
	}
	else
	{
		if(!flags[10]) //-g
			printf(" %3d",buf.st_uid);
		if(!flags[11] && !flags[22]) //-G,-o
			printf(" %3d",buf.st_gid);
	}
  
	if(!flags[12] && !flags[13]  && !flags[16]) //no  -h,-H,-k
		printf(" %10ld",buf.st_size);
	else if(flags[12]) //-h
	{
		if(buf.st_size > 1024)
		{
			a = (float)buf.st_size / 1024;
			printf(" %10.1fK", a);
		}
		else
			printf(" %10ld",buf.st_size);
	}
	else if(flags[16]) //-k
	{
		printf(" %10d",(int)buf.st_size / 1024 + 1);
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
  
	if(flags[4]) //-c
	{
		t = localtime(&buf.st_ctime);
		printf(" %4d-%02d-%02d %20d:%02d"  
			,t->tm_year+1900  
			,t->tm_mon+1  
			,t->tm_mday  
			,t->tm_hour  
			,t->tm_min);
	}
	else if(flags[32]) //-u
	{
		t = localtime(&buf.st_atime);
		printf(" %4d-%02d-%02d %02d:%02d"  
			,t->tm_year+1900  
			,t->tm_mon+1  
			,t->tm_mday  
			,t->tm_hour  
			,t->tm_min);
	}
	else
	{
		t = localtime(&buf.st_mtime);
		printf(" %4d-%02d-%02d %02d:%02d"  
			,t->tm_year+1900  
			,t->tm_mon+1  
			,t->tm_mday  
			,t->tm_hour  
			,t->tm_min);   
	}
         
	if(S_ISLNK(buf.st_mode) && flags[18]) //-L
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
*Input: buf[]: the information which have been read
*Output: nothing
*Return: 0: success
         -1: exit
****************************************************************************/
int HandleSort(struct stat sort[], char sname[LENGTH][LENGTH], int num)
{
	syslog(LOG_DEBUG,"%s L#%d  into HandleSort\n", __func__, __LINE__);
	if(flags[30]) //-t
		SortFlagt(sort, sname, num);
	if(flags[4]) //-c
		SortFlagc(sort, sname, num);
	if(flags[26]) //-r
		SortFlagr(sort, sname, num);
	if(flags[29]) //-S
		SortFlagS(sort, sname, num);
	if(flags[32]) //-u
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
	if(S_ISSOCK(buf->st_mode))
		printf("=");
	else if(S_ISLNK(buf->st_mode))
		printf("@");
	else if(S_ISFIFO(buf->st_mode))
		printf("|");
	else if(S_ISDIR(buf->st_mode) && !flags[23])
		printf("/");
	else if(buf->st_mode & S_IXUSR && !flags[23])
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
	printf("%ld\t",buf->st_ino);
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
			if(flags[25]) //-Q
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[01;34m\"%s\"\n\033[0m", path);
				else
					printf("\033[01;34m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[01;34m%s\n\033[0m", path);  
				else
					printf("\033[01;34m%s  \033[0m", path);  
			}
			break;  
		case S_IFCHR:     
			if(flags[25]) //-Q
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[40;01;33m\"%s\"\n\033[0m", path);
				else
					printf("\033[40;01;33m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[40;01;33m%s\n\033[0m", path);    
				else
					printf("\033[40;01;33m%s  \033[0m", path);    
			}  
			break;  
		case S_IFBLK:   
			if(flags[25]) //-Q
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[40;01;33m\"%s\"\n\033[0m", path);
				else
					printf("\033[40;01;33m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[40;01;33m%s\n\033[0m", path);    
				else
					printf("\033[40;01;33m%s  \033[0m", path);    
			}   
			break;  
		case S_IFIFO: 
			if(flags[25]) //-Q
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[40;33m\"%s\"\n\033[0m", path);
				else
					printf("\033[40;33m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[40;33m%s\n\033[0m", path);   
				else
					printf("\033[40;33m%s  \033[0m", path);  
			} 
			break;  
		case S_IFLNK:
			if(flags[25]) //-Q
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[01;36m\"%s\"\n\033[0m", path);
				else
					printf("\033[01;36m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[01;36m%s\n\033[0m", path); 
				else
					printf("\033[01;36m%s  \033[0m", path);   
			}   
			break;  
		case S_IFSOCK: 
			if(flags[25]) //-Q
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[01;35m\"%s\"\n\033[0m", path);
				else
					printf("\033[01;35m\"%s\"  \033[0m", path);
			}
			else
			{
				if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
					printf("\033[01;35m%s\n\033[0m", path);  
				else
					printf("\033[01;35m%s  \033[0m", path);     
			}    
			break;  
		default:
			if(buf->st_mode & S_IXUSR || buf->st_mode & S_IXGRP || buf->st_mode & S_IXOTH)
			{			
				if(flags[25]) //-Q
				{
					if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
						printf("\033[01;32m\"%s\"\n\033[0m", path);
					else
						printf("\033[01;32m\"%s\"  \033[0m", path);
				}
				else
				{
					if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
						printf("\033[01;32m%s\n\033[0m", path); 
					else
						printf("\033[01;32m%s  \033[0m", path); 
				} 
			}
			else
			{
				if(flags[25]) //-Q
				{
					if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
						printf("\"%s\"\n", path);
					else
						printf("\"%s\"  ", path);
				}
				else
				{
					if(flags[17] || flags[10] || flags[22] || flags[5]) //-l,-g,-o,-C
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

