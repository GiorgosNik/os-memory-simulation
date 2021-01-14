#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include "list.h"
struct memory{
	char* pageNum;
	int isWrite;
	int proccessId;
	int clock;
};
int main(int argc, char *argv[]){
	//Arguments are as such:
	//Algorith:0 or 1
	//Size of memory in GB
	//Queue length for each Process
	char* conversionPtr;
	int algorithm,memory,queue;
	int globalClock=0;
	size_t readSize;
	struct memory** frameTable;
	algorithm=strtol(argv[1],&conversionPtr,10);
	memory=strtol(argv[2],&conversionPtr,10);
	queue=strtol(argv[3],&conversionPtr,10);
	printf("Algorithm: %d Memory: %d Gb Queue Size: %d\n",algorithm,memory,queue);
	int sizeOfMemoryTable=memory*1024*1024*1024/4096;
	printf("Table size: %d\n",sizeOfMemoryTable);
	frameTable=malloc(sizeof(struct memory*));
	FILE* file1,*file2;
	file1=fopen("bzip.trace","r");
	file2=fopen("gcc.trace","r");
	char* line;
	int flag1=1, flag2=1;
	int i;
	int pageNum;
	while(flag1 || flag2){
		for(i=0;i<queue;i++){
			if(getline(&line,&readSize,file1)<0){
				flag1=0;
				break;
			}
			pageNum=strtol(line,&conversionPtr,16)>>12;
			printf("PageNum:%d Operation:%s",pageNum,conversionPtr);
		}
		for(i=0;i<queue;i++){
			if(getline(&line,&readSize,file2)){
				flag2=0;
				break;
			}
			pageNum=strtol(line,&conversionPtr,16)>>12;
			printf("PageNum:%d Operation:%s",pageNum,conversionPtr);
		}
	}
	return 0;
}
