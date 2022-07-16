#ifndef LIST
#define LIST
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
typedef struct list{
	int pageNum;
	int frame;
	struct list* next;
}list;
typedef struct table{
	struct list** buckets;
	int size;
	int id;
}table;
typedef struct memory{
	int pageNum;
	int isWrite;
	table* proccessId;
	int clock;
	int dirtyBit;
}memory;
int hash(int pageNum,int tableSize);
int addToTable(int pageNumber,table* hashTable,memory** frameTable,int tableSize,int clock,int isWrite,int algorithm);
struct table* initTable(int size,int id);
int victimize(memory** frameTable,int size,int algorithm);
int addToFrameTable(int pageNumber,table* hashTable,memory** frameTable,int tableSize,int clock,int isWrite,int algorithm);
int deleteFromHashTable(int pageNum,table* proccessId);
#endif //LIST
