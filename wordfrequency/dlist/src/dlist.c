#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<syslog.h>
#include<stdbool.h>


#include"dlist.h"
#include"../../wordfrequency/src/wordfrequency.h"

/*#define DEBUG_LOG(...)       \
        printf(__VA_ARGS__)  */

/****************************************************************************
*Function: CreateInitList
*Description: create and init list
*Input: nothing
*Output: nothing
*Return: list: head node
****************************************************************************/
List* CreateInitList()
{
	//openlog("CreateInitList_log", LOG_CONS | LOG_PID, LOG_USER);
	List* list = (List*)malloc(sizeof(List));

	if(!list)
		return NULL;
	
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	//syslog(LOG_DEBUG,"%s L#%d create and init success\n", __func__, __LINE__);
	//DEBUG_LOG("%s L#%d create and init success\n", __func__, __LINE__);

	//closelog();
	return list;
}

/****************************************************************************
*Function: Clean
*Description: use DeleteBack(list) to clean list
*Input: list: head node
*Output: nothing
*Return: nothing
****************************************************************************/
void Clean(List* list)
{
	while(DeleteBack(list) != -1 && DeleteBack(list) != 0);
}

/****************************************************************************
*Function: Destroy
*Description: destroy list
*Input: list: head node
*Output: nothing
*Return: nothing
****************************************************************************/
void Destroy(List* list)
{
	Clean(list);
	free(list);
}

/****************************************************************************
*Function: AddBack
*Description: add node at last of list
*Input: list: head node
           data: address of data
           datasize: size of data
*Output: nothing
*Return: -1: exit
             list->size: the size of list
****************************************************************************/
int AddBack(List* list, void* data, int datasize, int (*copy)())
{
	openlog("AddBack_log", LOG_CONS | LOG_PID, LOG_USER);
	Node* node = (Node*)malloc(sizeof(Node));
	
	if(!node)
		return -1;

	if(!data)
		return -1;

	//printf("input%d !!!!! %d %s\n", (*((Frequency*)data)).time, datasize, (*((Frequency*)data)).word);
	//printf("=====%d=====\n", datasize);
	node->data = (void*)malloc(datasize);
	//memcpy(node->data, data, datasize);
	/*memset(((Frequency*)(node->data))->word, 0, strlen(((Frequency*)data)->word)+1);
	
	memcpy(((Frequency*)(node->data))->word, ((Frequency*)data)->word, strlen(((Frequency*)data)->word));
	((Frequency*)(node->data))->time = ((Frequency*)data)->time;*/

	(*copy)(node->data, data);
	
	node->datasize = datasize;
	//printf("node->data:%p ##### data:%p\n", node->data, data);
	//printf("input%d @@@@@ %d %s\n", ((Frequency*)(node->data))->time, node->datasize, ((Frequency*)(node->data))->word);
	//node->data = data;
	syslog(LOG_DEBUG,"%s L#%d address of data %p\n", __func__, __LINE__, node->data);
	//DEBUG_LOG("%s L#%d address of data %p\n", __func__, __LINE__, node->data);
	
	if(IsEmpty(list))
	{
		node->prior = NULL;
		node->next = NULL;
		list->head = node;
		syslog(LOG_DEBUG,"%s L#%d address before add %p\n", __func__, __LINE__, list->tail);
		//DEBUG_LOG("%s L#%d address before add %p\n", __func__, __LINE__, list->tail);
	}
	else
	{
		list->tail->next = node;
		node->prior = list->tail;
		node->next = NULL;
		syslog(LOG_DEBUG,"%s L#%d address before add %p\n", __func__, __LINE__, list->tail);
		//DEBUG_LOG("%s L#%d address before add %p\n", __func__, __LINE__, list->tail);
	}
	list->tail = node;
	list->size++;

	syslog(LOG_DEBUG,"%s L#%d address after add %p\n", __func__, __LINE__, list->tail);
	//DEBUG_LOG("%s L#%d address after add %p\n", __func__, __LINE__, list->tail);

	closelog();
	return list->size;
}

/****************************************************************************
*Function: DeleteBack
*Description: delete the last node
*Input: list: head node
*Output: nothing
*Return: -1: exit
             list->size: the size of list
****************************************************************************/
int DeleteBack(List* list)
{
	openlog("DeleteBack_log", LOG_CONS | LOG_PID, LOG_USER);
	Node* temp;

	if(IsEmpty(list))
		return -1;

	temp = list->tail;
	//*datasize = list->tail->datasize;
	syslog(LOG_DEBUG,"%s L#%d address before delete %p\n", __func__, __LINE__, list->tail);
	//DEBUG_LOG("%s L#%d address before delete %p\n", __func__, __LINE__, list->tail);

	if(list->head == list->tail)
	{
		list->head = NULL;
		list->tail = NULL;
		syslog(LOG_DEBUG,"%s L#%d address after delete %p\n", __func__, __LINE__, list->tail);
		//DEBUG_LOG("%s L#%d address after delete %p\n", __func__, __LINE__, list->tail);
	}
	else
	{
		list->tail = list->tail->prior;
		list->tail->next = NULL;
		syslog(LOG_DEBUG,"%s L#%d address after delete %p\n", __func__, __LINE__, list->tail);
		//DEBUG_LOG("%s L#%d address after delete %p\n", __func__, __LINE__, list->tail);
	}
	list->size--;
	free(temp->data);
	free(temp);

	closelog();
	return list->size;
}

/****************************************************************************
*Function: Print
*Description: print list
*Input: list: head node
*Output: nothing
*Return: -1: exit
             0: success
****************************************************************************/
int Print(List* list)
{
	Node* temp;
	char buffer[1024] = "";
	int i;
	
	if(IsEmpty(list))
		return -1;

	temp = list->head;

	while(temp != list->tail)
	{
		//prinft("%p ", temp->data);
		for(i = 0; i < temp->datasize; i++)
		{
			if((*(char*)(temp->data+i)) <= 31 || (*(char*)(temp->data+i)) >= 127)
				sprintf (buffer + strlen (buffer), "%02X", *(unsigned char*)(temp->data+i));
			else
				sprintf (buffer + strlen (buffer), "%c", *(char*)(temp->data+i));
		}
		//printf("%d ", *(int*)temp->data);
		sprintf (buffer + strlen (buffer), "->");
		temp = temp->next;
	}
	//prinft("%p\n ", list->tail);
	for(i = 0; i < temp->datasize; i++)
	{
		if(*(char*)(temp->data+i) <= 31 || *(char*)(temp->data+i) >= 127)
			sprintf (buffer + strlen (buffer), "%02X", *(unsigned char*)(temp->data+i));
		else
			sprintf (buffer + strlen (buffer), "%c", *(char*)(temp->data+i));
	}

	//printf("%d\n ", *(int*)temp->data);
	printf("%s\n", buffer);
	
	return 0;
}

/****************************************************************************
*Function: FPrint
*Description: fprint list
*Input: list: head node
*Output: nothing
*Return: -1: exit
             0: success
****************************************************************************/
int FPrint(List* list)
{
	Node* temp;
	char buffer[1024] = "";
	int i;
	
	if(IsEmpty(list))
		return -1;

	temp = list->tail;

	while(temp != list->head)
	{
		//prinft("%p ", temp->data);
		for(i = 0; i < temp->datasize; i++)
		{
			if((*(char*)(temp->data+i)) <= 31 || (*(char*)(temp->data+i)) >= 127)
				sprintf (buffer + strlen (buffer), "%02X", *(unsigned char*)(temp->data+i));
			else
				sprintf (buffer + strlen (buffer), "%c", *(char*)(temp->data+i));
		}
		sprintf (buffer + strlen (buffer), "->");
		temp = temp->prior;
	}
	//prinft("%p\n ", list->head);
	for(i = 0; i < temp->datasize; i++)
	{
		if((*(char*)(temp->data+i)) <= 31 || (*(char*)(temp->data+i)) >= 127)
			sprintf (buffer + strlen (buffer), "%02X", *(unsigned char*)(temp->data+i));
		else
			sprintf (buffer + strlen (buffer), "%c", *(char*)(temp->data+i));
	}
	printf("%s\n", buffer);

	return 0;
}

/****************************************************************************
*Function: FindData
*Description: find node and return address of data
*Input: list: head node
           node_num: the node that user need to find
*Output: datasize: size of data
*Return: NULL: exit
             temp->data: address of data that user need to find
****************************************************************************/
void* FindData(List* list, int node_num, int* datasize)
{
	openlog("FindData_log", LOG_CONS | LOG_PID, LOG_USER);
	Node* temp;
	int i;
	
	if(IsEmpty(list))
		return NULL;

	temp = list->head;
	syslog(LOG_DEBUG,"%s L#%d  %p\n", __func__, __LINE__, list->head);
	//DEBUG_LOG("%s L#%d  %p\n", __func__, __LINE__, list->head);

	if(node_num > list->size)
		return NULL;
	else
	{
		for(i = 1; i < node_num; i++)
		{
			temp = temp->next;
			syslog(LOG_DEBUG,"%s L#%d  %p*******\n", __func__, __LINE__, temp);
			//DEBUG_LOG("%s L#%d  %p*******\n", __func__, __LINE__, temp);
		}
	}
	*datasize = temp->datasize;
	closelog();
	return temp->data;
}

/****************************************************************************
*Function: DeleteData
*Description: delete node
*Input: list: head node
           node_num:  the node that user need to delete
*Output: nothing
*Return: -1: exit
             list->size: the size of list
****************************************************************************/
int DeleteData(List* list, int node_num)
{
	openlog("DeleteData_log", LOG_CONS | LOG_PID, LOG_USER);
	Node* temp;
	int i;

	if(IsEmpty(list))
		return -1;

	temp = list->head;
	//*datasize = list->tail->datasize;
	syslog(LOG_DEBUG,"%s L#%d address before delete %p\n", __func__, __LINE__, list->head);
	//DEBUG_LOG("%s L#%d address before delete %p\n", __func__, __LINE__, list->tail);

	if(node_num > list->size)
		return -1;
	else
	{
		for(i = 1; i < node_num; i++)
		{
			temp = temp->next;
			syslog(LOG_DEBUG,"%s L#%d  %p#######\n", __func__, __LINE__, temp);
			//DEBUG_LOG("%s L#%d  %p*******\n", __func__, __LINE__, temp);
		}
	}

	if(list->head == list->tail)
	{
		list->head = NULL;
		list->tail = NULL;
		syslog(LOG_DEBUG,"%s L#%d address after delete %p\n", __func__, __LINE__, list->tail);
		//DEBUG_LOG("%s L#%d address after delete %p\n", __func__, __LINE__, list->tail);
	}
	else if(temp == list->tail)
	{
		list->tail = list->tail->prior;
		list->tail->next = NULL;
		syslog(LOG_DEBUG,"%s L#%d address after delete %p\n", __func__, __LINE__, list->tail);
		//DEBUG_LOG("%s L#%d address after delete %p\n", __func__, __LINE__, list->tail);
	}
	else
	{
		temp->prior->next = temp->next;
		temp->next->prior = temp->prior;
	}
	list->size--;
	free(temp->data);
	free(temp);

	closelog();
	return list->size;

}

/****************************************************************************
*Function: IsEmpty
*Description: whether the list is empty
*Input: list: head node
*Output: nothing
*Return: 0: not empty 
             non 0: empty
****************************************************************************/
int IsEmpty(List* list)
{
	return list->size == 0;
}

