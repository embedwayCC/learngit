#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<syslog.h>
#include<stdbool.h>

#include"../../dlist/src/dlist.h"
#include"wordfrequency.h"

//typedef int (*copy)(void* data1, void* data2);

List* list = NULL;

int CopyFrequency(void* data1, void* data2)
{
	if(data1 == NULL || data2 == NULL)
		return -1;
	
	memset(((Frequency*)data1)->word, 0, WORD_LENGTH);
	memcpy(((Frequency*)data1)->word, ((Frequency*)data2)->word, strlen(((Frequency*)data2)->word));
	
	((Frequency*)data1)->time = ((Frequency*)data2)->time;
	
	return 0;
}

void HandleWord(char* word, bool flag)
{
	openlog("HandleWord_log", LOG_CONS | LOG_PID, LOG_USER);
	int i, j;
	//char* word = (char*)malloc(strlen(temp));

	//memcpy(word, temp,strlen(temp));
	//printf("%s=====\n",word);
	if(flag == 1)
	{
		//printf("ignore\n");
		for(i = 0; i < strlen(word); i++)
		{
			if(*(word + i) >= 'A' && *(word + i) <= 'Z')
			{
				*(word + i) += 32;        
			}
			if(!((*(word + i) >= 'A' && *(word + i) <= 'Z') 
				|| (*(word + i) >= 'a' && *(word + i) <= 'z') 
				|| *(word + i) == '-'
				|| *(word + i) == '\''))
			{
				for(j = i; *(word + j) != '\0'; j++)
				{
					*(word + j) = *(word + j + 1);
				}
				i--;
			}

		}
	}else
	{
		//printf("don't ignore\n");
		for(i = 0; i < strlen(word); i++)
		{
			if(!((*(word + i) >= 'A' && *(word + i) <= 'Z') 
				|| (*(word + i) >= 'a' && *(word + i) <= 'z') 
				|| *(word + i) == '-'
				|| *(word + i) == '\''))
			{
				for(j = i; *(word + j) != '\0'; j++)
					*(word + j) = *(word + j + 1);
				i--;
			}

		}
	}
	for(i = 0; i < strlen(word); i++)
		{
			if(*word == '\'' || *word == '-')
			{
				for(j = 0; *(word + j) != '\0'; j++)
					*(word + j) = *(word + j + 1); 
			}
			else
				break;
		}
	for(i = 0; i < strlen(word); i++)
		{
			if(*(word+strlen(word)-1-i) == '\'' || *(word+strlen(word)-1-i) == '-')
			{
				*(word+strlen(word)-1-i)='\0';
				i--;
			}
			else
				break;
		}
	syslog(LOG_DEBUG,"%s L#%d  %s\n", __func__, __LINE__, word);
	closelog();
	//printf("HandleWord\n");
	//printf("%s=====+++\n",word);
	//return word;
}
int SearchWord(char* word, bool* flag)
{
	openlog("SearchWord_log", LOG_CONS | LOG_PID, LOG_USER);
	Node* temp = NULL;

	if(IsEmpty(list))
		*flag = 0;
	else
	{
		temp = list->head;
		syslog(LOG_DEBUG,"%s L#%d  %p\n", __func__, __LINE__, list->head);
		//DEBUG_LOG("%s L#%d  %p\n", __func__, __LINE__, list->head);

		do
		{
			//printf("===============\n");
			//printf("read  %s###\n", ((Frequency*)(temp->data))->word);
			//printf("read  %d###\n", ((Frequency*)(temp->data))->time);
			if(strcmp(((Frequency*)(temp->data))->word, word) == 0)
			{
				*flag = 1;
				(((Frequency*)(temp->data))->time)++;
				//printf("jump time:%d", ((Frequency*)(temp->data))->time);
				break;
			}
			//printf("strcmp\n");
			temp = temp->next;
		}while(temp != NULL);
		//printf("---------------\n");
	}
	closelog();
	return 0;
}

int CountWordByPosition(char* word)
{
	openlog("CountWordByPosition_log", LOG_CONS | LOG_PID, LOG_USER);
	bool flag = 0;
	SearchWord(word, &flag);
	syslog(LOG_DEBUG,"%s L#%d  %d\n", __func__, __LINE__, flag);
	if(flag == 0)
	{
		Frequency* frequency = (Frequency*)malloc(sizeof(Frequency));
		memset(frequency->word, 0, WORD_LENGTH);
		strcpy(frequency->word, word);
		frequency->time = 1;
		AddBack(list, frequency, sizeof(Frequency), CopyFrequency);
		free(frequency);
	}
	closelog();
	//free(word);
	return 0;
}
int PrintResult()
{
	Node* temp;
	int i = 0;
	
	if(IsEmpty(list))
		return -1;

	temp = list->head;

	do
	{
		printf("%-25s->%-5d|    ", ((Frequency*)(temp->data))->word, ((Frequency*)(temp->data))->time);
		i++;
		if(i % 5 == 0)
			printf("\n");
		temp = temp->next;
	}while(temp != NULL);
	printf("\n");

	//Print(list);
	//Destroy(list);

	return 0;
}

int FPrintResult()
{
	Node* temp;
	int i = 0;
	
	if(IsEmpty(list))
		return -1;

	temp = list->tail;

	do
	{
		printf("%-25s->%-5d|    ", ((Frequency*)(temp->data))->word, ((Frequency*)(temp->data))->time);
		i++;
		if(i % 5 == 0)
			printf("\n");
		temp = temp->prior;
	}while(temp != NULL);
	printf("\n");

	//Print(list);
	//Destroy(list);

	return 0;
}

int PositionToDictionary()
{
	Node* temp, *corrent;

	Node* mid = (Node*)malloc(sizeof(Node));
	mid->data = (void*)malloc(sizeof(Frequency));
	memset(((Frequency*)(mid->data))->word, 0, WORD_LENGTH);
	((Frequency*)(mid->data))->time = 1;

	if(IsEmpty(list))
		return -1;

	temp = list->head->next;
	corrent = list->head;

	while(temp != NULL)
	{
		while(temp != NULL)
		{
			if(strcmp(((Frequency*)(corrent->data))->word, ((Frequency*)(temp->data))->word) > 0)
			{
				strcpy(((Frequency*)(mid->data))->word, ((Frequency*)(temp->data))->word);
				strcpy(((Frequency*)(temp->data))->word, ((Frequency*)(corrent->data))->word);
				strcpy(((Frequency*)(corrent->data))->word, ((Frequency*)(mid->data))->word);

				((Frequency*)(mid->data))->time = ((Frequency*)(temp->data))->time;
				((Frequency*)(temp->data))->time = ((Frequency*)(corrent->data))->time;
				((Frequency*)(corrent->data))->time = ((Frequency*)(mid->data))->time;
				//printf("change\n");
			}
			temp = temp->next;
		}
		corrent = corrent->next;
		temp = corrent->next;
	}
	free(mid->data);
	free(mid);
	PrintResult();
	//FPrintResult();

	//Destroy(list);
}

int PositionToTimes()
{
	Node* temp, *corrent;

	Node* mid = (Node*)malloc(sizeof(Node));
	mid->data = (void*)malloc(sizeof(Frequency));
	memset(((Frequency*)(mid->data))->word, 0, WORD_LENGTH);
	((Frequency*)(mid->data))->time = 1;

	if(IsEmpty(list))
		return -1;

	temp = list->head->next;
	corrent = list->head;

	while(temp != NULL)
	{
		while(temp != NULL)
		{
			if(((Frequency*)(corrent->data))->time > ((Frequency*)(temp->data))->time)
			{
				strcpy(((Frequency*)(mid->data))->word, ((Frequency*)(temp->data))->word);
				strcpy(((Frequency*)(temp->data))->word, ((Frequency*)(corrent->data))->word);
				strcpy(((Frequency*)(corrent->data))->word, ((Frequency*)(mid->data))->word);

				((Frequency*)(mid->data))->time = ((Frequency*)(temp->data))->time;
				((Frequency*)(temp->data))->time = ((Frequency*)(corrent->data))->time;
				((Frequency*)(corrent->data))->time = ((Frequency*)(mid->data))->time;
				//printf("change\n");
			}
			temp = temp->next;
		}
		corrent = corrent->next;
		temp = corrent->next;
	}
	free(mid->data);
	free(mid);
	PrintResult();
	FPrintResult();

	//Destroy(list);
}
void InitList()
{
	list = CreateInitList();
}

Node* Partition(List* list, Node* low, Node* high, char* type)
{
	openlog("Partition_log", LOG_CONS | LOG_PID, LOG_USER);
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->data = (void*)malloc(sizeof(Frequency));
	memset(((Frequency*)(temp->data))->word, 0, WORD_LENGTH);
	((Frequency*)(temp->data))->time = 1;
	
	Node* low_= (Node*)malloc(sizeof(Node));
	low_->data = (void*)malloc(sizeof(Frequency));
	memset(((Frequency*)(low_->data))->word, 0, WORD_LENGTH);
	((Frequency*)(low_->data))->time = 1; 
	
	if (IsEmpty(list))
		return NULL;
	
	if (list->head == list->tail) 
	{  
		return list->head;
	}
	strcpy(((Frequency*)(low_->data))->word, ((Frequency*)(low->data))->word);
	((Frequency*)(low_->data))->time = ((Frequency*)(low->data))->time;

	if(strcmp("dictionary",type)==0)
	{
		syslog(LOG_DEBUG,"%s L#%d  type=%s\n", __func__, __LINE__, type);
		while(low != high)
		{
			while((low != high) && (strcmp(((Frequency*)(high->data))->word, ((Frequency*)(low_->data))->word) >= 0))
			{
				high = high->prior;
			}
			//printf("%dlow----%dhigh\n",sizeof(((Frequency*)(low->data))->word),sizeof(((Frequency*)(high->data))->word));
			strcpy(((Frequency*)(temp->data))->word, ((Frequency*)(low->data))->word);
			strcpy(((Frequency*)(low->data))->word, ((Frequency*)(high->data))->word);
			strcpy(((Frequency*)(high->data))->word, ((Frequency*)(temp->data))->word);

			((Frequency*)(temp->data))->time = ((Frequency*)(low->data))->time;
			((Frequency*)(low->data))->time = ((Frequency*)(high->data))->time;
			((Frequency*)(high->data))->time = ((Frequency*)(temp->data))->time;

			while((low != high) && (strcmp(((Frequency*)(low->data))->word, ((Frequency*)(low_->data))->word) <= 0))
			{
				low= low->next;
			}
			strcpy(((Frequency*)(temp->data))->word, ((Frequency*)(low->data))->word);
			strcpy(((Frequency*)(low->data))->word, ((Frequency*)(high->data))->word);
			strcpy(((Frequency*)(high->data))->word, ((Frequency*)(temp->data))->word);

			((Frequency*)(temp->data))->time = ((Frequency*)(low->data))->time;
			((Frequency*)(low->data))->time = ((Frequency*)(high->data))->time;
			((Frequency*)(high->data))->time = ((Frequency*)(temp->data))->time;
		}
	}
	else if(strcmp("time",type)==0)
	{
		syslog(LOG_DEBUG,"%s L#%d  type=%s\n", __func__, __LINE__, type);
		while(low != high)
		{
			while((low != high) && (((Frequency*)(high->data))->time >= ((Frequency*)(low_->data))->time))
			{
				high = high->prior;
			}
			strcpy(((Frequency*)(temp->data))->word, ((Frequency*)(low->data))->word);
			strcpy(((Frequency*)(low->data))->word, ((Frequency*)(high->data))->word);
			strcpy(((Frequency*)(high->data))->word, ((Frequency*)(temp->data))->word);

			((Frequency*)(temp->data))->time = ((Frequency*)(low->data))->time;
			((Frequency*)(low->data))->time = ((Frequency*)(high->data))->time;
			((Frequency*)(high->data))->time = ((Frequency*)(temp->data))->time;

			while((low != high) && (((Frequency*)(low->data))->time <= ((Frequency*)(low_->data))->time))
			{
				low= low->next;
			}
			strcpy(((Frequency*)(temp->data))->word, ((Frequency*)(low->data))->word);
			strcpy(((Frequency*)(low->data))->word, ((Frequency*)(high->data))->word);
			strcpy(((Frequency*)(high->data))->word, ((Frequency*)(temp->data))->word);

			((Frequency*)(temp->data))->time = ((Frequency*)(low->data))->time;
			((Frequency*)(low->data))->time = ((Frequency*)(high->data))->time;
			((Frequency*)(high->data))->time = ((Frequency*)(temp->data))->time;
		}
	}
	else
		return NULL;
	/*if(strcmp("time",type)==0)
	{
	printf("----------------------------------------------------------\n");
	while(low != high)
	{
		while((low != high) && (((Frequency*)(high->data))->time >= ((Frequency*)(low_->data))->time))
		{
			high = high->prior;
		}
		strcpy(((Frequency*)(temp->data))->word, ((Frequency*)(low->data))->word);
		strcpy(((Frequency*)(low->data))->word, ((Frequency*)(high->data))->word);
		strcpy(((Frequency*)(high->data))->word, ((Frequency*)(temp->data))->word);

		((Frequency*)(temp->data))->time = ((Frequency*)(low->data))->time;
		((Frequency*)(low->data))->time = ((Frequency*)(high->data))->time;
		((Frequency*)(high->data))->time = ((Frequency*)(temp->data))->time;

		while((low != high) && (((Frequency*)(low->data))->time <= ((Frequency*)(low_->data))->time))
		{
			low= low->next;
		}
		strcpy(((Frequency*)(temp->data))->word, ((Frequency*)(low->data))->word);
		strcpy(((Frequency*)(low->data))->word, ((Frequency*)(high->data))->word);
		strcpy(((Frequency*)(high->data))->word, ((Frequency*)(temp->data))->word);

		((Frequency*)(temp->data))->time = ((Frequency*)(low->data))->time;
		((Frequency*)(low->data))->time = ((Frequency*)(high->data))->time;
		((Frequency*)(high->data))->time = ((Frequency*)(temp->data))->time;
	}
	}*/
	free(temp->data);
	free(temp);
	free(low_->data);
	free(low_);
	closelog();
	return low;
}
void QuickSort(List* list, Node* low, Node* high, char* type)
{
	Node* mid = Partition(list, low, high, type);
	if(low != mid)		
	{
		QuickSort(list, low, mid->prior, type);
	}
	if ( high!= mid)
	{
		QuickSort(list, mid->next, high, type);
	}
}
void PrintQuickSort(char* type)
{
	QuickSort(list,list->head,list->tail,type);
	PrintResult();
	FPrintResult();
}
void DestroyList()
{
	Destroy(list);
}
