#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<syslog.h>

#include"dlist.h"

/*#define DEBUG_LOG(...)       \
        printf(__VA_ARGS__)  */
                                        
/* create and init list */
List* CreateInitList()
{
	openlog("CreateInitList_log", LOG_CONS | LOG_PID, LOG_USER);
	List* list = (List*)malloc(sizeof(List));

	if(!list)
		return NULL;
	
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	syslog(LOG_DEBUG,"%s L#%d create and init success\n", __func__, __LINE__);
	//DEBUG_LOG("%s L#%d create and init success\n", __func__, __LINE__);

	closelog();
	return list;
}

/* clean list */
void Clean(List* list)
{
	while(DeleteBack(list));
}

/* destroy list */
void Destroy(List* list)
{
	Clean(list);
	free(list);
}

/* add node at last of list */
int AddBack(List* list, void* data)
{
	openlog("AddBack_log", LOG_CONS | LOG_PID, LOG_USER);
	Node* node = (Node*)malloc(sizeof(Node));
	
	if(!node)
		return -1;
	
	node->data = data;
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

/* delete the last node */
void* DeleteBack(List* list)
{
	openlog("DeleteBack_log", LOG_CONS | LOG_PID, LOG_USER);
	Node* temp;
	void* data;

	if(IsEmpty(list))
		return NULL;

	temp = list->tail;
	data = list->tail->data;
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
	free(temp);

	closelog();
	return data;
}

/* print */
int Print(List* list)
{
	Node* temp;
	char buffer[1024] = "";
	
	if(IsEmpty(list))
		return -1;

	temp = list->head;

	while(temp != list->tail)
	{
		//prinft("%p ", temp->data);
		sprintf (buffer + strlen (buffer), "%02X++", *(unsigned char*)temp->data);
		//printf("%d ", *(int*)temp->data);
		temp = temp->next;
	}
	//prinft("%p\n ", list->tail);
	sprintf (buffer + strlen (buffer), "%02X\n", *(unsigned char*)temp->data);
	//printf("%d\n ", *(int*)temp->data);
	printf("%s\n", buffer);
	
	return 0;
}

/* fprint */
int FPrint(List* list)
{
	Node* temp;
	char buffer[1024] = "";
	
	if(IsEmpty(list))
		return -1;

	temp = list->tail;

	while(temp != list->head)
	{
		//prinft("%p ", temp->data);
		sprintf (buffer + strlen (buffer), "%02X--", *(unsigned char*)temp->data);
		temp = temp->prior;
	}
	//prinft("%p\n ", list->head);
	sprintf (buffer + strlen (buffer), "%02X\n", *(unsigned char*)temp->data);
	printf("%s\n", buffer);

	return 0;
}

/* find node*/ 
void* FindData(List* list, int node_num)
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
	closelog();
	return temp->data;
}

/* whether the list is empty*/
int IsEmpty(List* list)
{
	return list->size == 0;
}

