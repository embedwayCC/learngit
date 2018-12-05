#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"dlist.h"

int main()
{
	List* list = CreateInitList();

	int array1[10];
	int array2[10];
	int i;

	for(i = 0; i < 10; i++)
	{
		array1[i] = i*3 + 1;
		array2[i] = i*5 + 1;
		AddBack(list, &array1[i]);
	}

	printf("Print:\n");
	Print(list);
	printf("FPrint:\n");
	FPrint(list);

	for(i = 0; i < 10; i++)
		AddBack(list, &array2[i]);

	printf("Print after AddBack array2:\n");
	Print(list);
	printf("FPrint after AddBack array2:\n");
	FPrint(list);

	i = *(int*)FindData(list, 5);
	printf("the data of 5th node is %d:\n", i);

	DeleteBack(list);
	printf("Print after DeleteBack:\n");
	Print(list);

	printf("Clean list:\n");
	Clean(list);

	i = list->size;
	printf("after clean list,list->size:%d\n",i);
        
	printf("Destroy list:\n");
	Destroy(list);
        
	return 0;
}

