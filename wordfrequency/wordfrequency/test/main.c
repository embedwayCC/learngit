#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>
#include<time.h>


#include"../../dlist/src/dlist.h"
#include"wordfrequency.h"

int main(int argc, char* argv[])
{
	//List* list = CreateInitList();
	//int a = 5;
	char temp[WORD_LENGTH]={0};
	//char* word = NULL;
	char dictionary[WORD_LENGTH]="dictionary";
	char time[WORD_LENGTH]="time";
	int opt;
	FILE *fp;
	bool flag;
	char address[WORD_LENGTH]={0};
	//struct timeval timeStart, timeEnd;
	//double runTime=0;
	int i;

	while((opt = getopt(argc, argv, "a:")) != -1){
		switch(opt){
			case 'a':
				//for(i=0;i<100;i++)
				//{
					flag = atoi(optarg);
					strcpy(address,argv[optind]);
					//fp = fopen("/home/debian/wordfrequency/article/testarticle.txt", "r");
					fp = fopen(address, "r");
					if(fp == NULL)
					{
						printf("read error!\n");
						exit(1);
					}
					InitList();
					while((fscanf(fp,"%s",temp))!= EOF){
						//printf("%s\n",temp);
						//word = HandleWord(temp, flag);
						HandleWord(temp, flag);
						//printf("%s====---------\n",word);
						if(strlen(temp) != 0)
						//count = wordCount(temp,num);
						//AddBack(list, temp, strlen(temp));
							CountWordByPosition(temp);
						memset(temp, 0, WORD_LENGTH);
						//word = NULL;
					}
					//printf("position:\n");
					//gettimeofday(&timeStart, NULL );
					//PrintResult();
					//gettimeofday(&timeEnd, NULL );
					//runTime = (timeEnd.tv_sec - timeStart.tv_sec ) + (double)(timeEnd.tv_usec -timeStart.tv_usec)/1000000;
					//printf("runTime is %lf\n", runTime); 
					//FPrintResult();
					//PositionToDictionary();
					//PositionToTimes();
					//printf("dictionary:\n");
					//gettimeofday(&timeStart, NULL );
					PrintQuickSort(dictionary);
					//PositionToDictionary();
					//gettimeofday(&timeEnd, NULL );
					//runTime = (timeEnd.tv_sec - timeStart.tv_sec ) + (double)(timeEnd.tv_usec -timeStart.tv_usec)/1000000;
					//printf("runTime is %lf\n", runTime); 
					//FPrintResult();
					//printf("time:\n");
					//PrintQuickSort(time);
					DestroyList();
					fclose(fp);
					//free(fp);
				//}
				break;
		}
	}	
	return 0;
}


