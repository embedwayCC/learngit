/* dlist.h */

#ifndef DLIST_H
#define DLIST_H

typedef struct pNode{
	void* data;
	int datasize;
	struct pNode* prior;
	struct pNode* next;
}Node;

typedef struct dList{
	Node* head;
	Node* tail;
	int size;
}List;

/* create and init list */
extern List* CreateInitList();

/* clean list */
extern void Clean(List* list);

/* destroy list */
extern void Destroy(List* list);

/* add node at last of list */
extern int AddBack(List* list, void* data, int datasize);

/* delete the last node */
extern int DeleteBack(List* list);

/* print */
extern int Print(List* list);

/* fprint */
extern int FPrint(List* list);

/* find node*/ 
extern void* FindData(List* list, int node_num, int* datasize);

/* delete data */
extern int DeleteData(List* list, int node_num);

/* whether the list is empty*/
extern int IsEmpty(List* list);

#endif

