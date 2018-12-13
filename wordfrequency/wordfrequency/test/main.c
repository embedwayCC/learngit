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
	char temp[200]={0};
	char type1[20]="dictionary";
	char type2[20]="time";
	int opt;
	FILE *fp;
	//struct timeval timeStart, timeEnd;
	//double runTime=0;
	//int i;

	while((opt = getopt(argc, argv, "a")) != -1){
		switch(opt){
			case 'a':
				//for(i=0;i<100;i++)
				//{
					fp = fopen("/home/chencheng/wordfrequency/article/1.Harry Potter and the Sorcerers Stone.txt", "r");
					if(fp == NULL)
					{
						printf("read error!\n");
						exit(1);
					}
					InitList();
					while((fscanf(fp,"%s",temp))!= EOF){
						//printf("%s\n",temp);
						HandleWord(temp, 1);
						//printf("%s\n",temp);
						if(strlen(temp) != 0)
						//count = wordCount(temp,num);
						//AddBack(list, temp, strlen(temp));
							CountWordByPosition(temp);
					}
					//printf("position:\n");
					//gettimeofday(&timeStart, NULL );
					PrintResult();
					//gettimeofday(&timeEnd, NULL );
					//runTime = (timeEnd.tv_sec - timeStart.tv_sec ) + (double)(timeEnd.tv_usec -timeStart.tv_usec)/1000000;
					//printf("runTime is %lf\n", runTime); 
					FPrintResult();
					//PositionToDictionary();
					//PositionToTimes();
					//printf("dictionary:\n");
					//gettimeofday(&timeStart, NULL );
					PrintQuickSort(type1);
					//PositionToDictionary();
					//gettimeofday(&timeEnd, NULL );
					//runTime = (timeEnd.tv_sec - timeStart.tv_sec ) + (double)(timeEnd.tv_usec -timeStart.tv_usec)/1000000;
					//printf("runTime is %lf\n", runTime); 
					//FPrintResult();
					//printf("time:\n");
					PrintQuickSort(type2);
					DestroyList();
				//}
				break;
		}
	}	
	return 0;
}


