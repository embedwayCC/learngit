#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include"dlist.h"

/*void add(List* list)
{
	void* data = NULL;
	int a = 5;
	char b = 'B';

	printf("%p", &data);
	data = &a;
	AddBack(list, data);
	data = &b;
	AddBack(list, data);
}
void print(List* list)
{
	Print(list);
}*/

int main(int argc, char* argv[])
{
	/*List* list = CreateInitList();

	int a = 5, m;
	char b = 'B', i;
	char c[20] = "hello world!", n[20] = "";
	int datasize;
	//char yyy='5';

	AddBack(list, &a, sizeof(a));
	AddBack(list, &b, sizeof(b));
	AddBack(list, c, strlen(c));

	printf("Print:\n");
	Print(list);
	printf("FPrint:\n");
	FPrint(list);

	FindData(list, 3, &datasize);
	printf("datasize of data user is finding is:%d\n", datasize);
	memcpy(n, FindData(list, 3, &datasize), datasize);
	printf("the data of 2th node is:%s\n", n);

	DeleteBack(list);
	printf("Print after DeleteBack:\n");
	Print(list);

	printf("Clean list:\n");
	Clean(list);

	m = list->size;
	printf("after clean list,list->size:%d\n",m);
        
	printf("Destroy list:\n");
	Destroy(list);

	//void* data = NULL;
	//data = &a;
	//add(list, data);
	//data = &b;
	//add(list, data);

	//add(list);
	//print(list);

	//printf("%d   %d:\n",a,yyy);*/

	List* list = CreateInitList();
	int opt;
	char c[20] = "";
	int i, j, max;
	char hello[10] = "hello";
	char b = 'B';
	int a = 5;
	int datasize;
	char n[20] = "";
	
	while((opt = getopt(argc, argv, "a:x:dpcefs:r:")) != -1){
		switch(opt){
			case 'a':
				printf("AddBack list:\n");
				//printf("The argument of -a is %s\n", optarg);
				//printf("The length of -a is %d\n", strlen(optarg));
				strcpy(c,optarg);
				//printf("The value of c[20] is %s\n", c);
				AddBack(list, &c, strlen(c));
				printf("Print after AddBack:\n");
				Print(list);
				printf("\n");
				break;
			case 'x':
				//printf("The argument of -a is %s\n", optarg);
				max = atoi(optarg);
				//printf("%d",max);
				//memcpy(c,optarg,strlen(optarg)+1);
				//printf("The value of c[20] is %s\n", c);
				for(j = 0; j < max; j++)
				{	
					List* list1 = CreateInitList();
					
					AddBack(list1, &hello, strlen(hello));
					AddBack(list1, &a,sizeof(a));
					AddBack(list1, &b,sizeof(b));
					AddBack(list1, &hello, strlen(hello));
					AddBack(list1, &a,sizeof(a));
					AddBack(list1, &a,sizeof(a));
					AddBack(list1, &b,sizeof(b));
					AddBack(list1, &hello, strlen(hello));
					AddBack(list1, &a,sizeof(a));
					AddBack(list1, &b,sizeof(b));
					printf("Print after AddBack:\n");
					Print(list1);
					printf("\n");

					DeleteBack(list1);
					printf("Print after DeleteBack:\n");
					Print(list1);
					printf("\n");

					FindData(list1, 4, &datasize);
					printf("datasize of data user is finding is:%d\n", datasize);
					memcpy(n, FindData(list1, 4, &datasize), datasize);
					printf("the data of 4th node is:%s\n", n);
					printf("\n");

					DeleteData(list1, 2);
					printf("Print after DeleteData:\n");
					Print(list1);
					printf("\n");

					Clean(list1);
					i = list1->size;
					printf("after clean list,list->size:%d\n",i);
					printf("\n");

					printf("Destroy list:\n");
					Destroy(list1);
					printf("\n");
				}
				
				//for(i = 10; i < 20; i++)
					//AddBack(list, &b, sizeof(b));
				//for(i = 20; i < max; i++)
					//AddBack(list, &a, sizeof(a));
				//printf("Print after ax:\n");
				//Print(list);
				break;
			case 'd':
				printf("DeleteBack list:\n");
				DeleteBack(list);
				printf("Print after DeleteBack:\n");
				Print(list);
				printf("\n");
				break;
			case 'p':
				printf("Print:\n");
				Print(list);
				printf("\n");
				break;
			case 'f':
				printf("FPrint:\n");
				FPrint(list);
				printf("\n");
				break;
			case 's':
				i = atoi(optarg);
				printf("FindData:\n");
				printf("data user is finding is:%d\n", i);
				FindData(list, i, &datasize);
				printf("datasize of data user is finding is:%d\n", datasize);
				memcpy(n, FindData(list, i, &datasize), datasize);
				printf("the data of %dth node is:%s\n", i, n);
				printf("\n");
				break;
			case 'c':
				printf("Clean list:\n");
				Clean(list);
				i = list->size;
				printf("after clean list,list->size:%d\n",i);
				printf("\n");
				break;
			case 'e':
				printf("Destroy list:\n");
				Destroy(list);
				printf("\n");
				break;
			case 'r':
				i = atoi(optarg);
				printf("DeleteData:\n");
				printf("data user is deleting is:%d\n", i);
				DeleteData(list, i);
				printf("Print after DeleteData:\n");
				Print(list);
				printf("\n");
				break;
		}
	}
	
	return 0;
}

