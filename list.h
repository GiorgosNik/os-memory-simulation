#ifndef LIST
#define LIST
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
struct list{
	char* pageNum;
	int frame;
	struct list* next;
};
struct table{
	struct list** buckets;
};
typedef struct list list;
int addToMem(char* pageNumber,list* node);
int initTable(struct table*);

#endif //LIST
