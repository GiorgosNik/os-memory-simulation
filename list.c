#include "list.h"
extern int reads;
extern int writes;
extern int faults;
int addToTable(int pageNumber,struct table* hashTable,memory** frameTable,int tableSize,int clock,int isWrite,int algorithm){
	list* newNode;
	int i=0;
	list* node=hashTable->buckets[hash(pageNumber,hashTable->size)];
	if(hashTable->buckets[hash(pageNumber,hashTable->size)]==NULL){		//If it is the first in this bucket we create the first list node,
		faults++;														//increase the fault count, and add it by calling addToFrameTable
		hashTable->buckets[hash(pageNumber,hashTable->size)]=malloc(sizeof(list));
		hashTable->buckets[hash(pageNumber,hashTable->size)]->pageNum=pageNumber;
		hashTable->buckets[hash(pageNumber,hashTable->size)]->frame=addToFrameTable(pageNumber,hashTable,frameTable,tableSize,clock,isWrite,algorithm);
		hashTable->buckets[hash(pageNumber,hashTable->size)]->next=NULL;
		return 0;
	}
	node=hashTable->buckets[hash(pageNumber,hashTable->size)];
	while(node!=NULL){												//If there is at least one list node, search to fint the page
		if(node->pageNum==pageNumber){								//If found, set the clock and the second Chance marker
			frameTable[node->frame]->clock=clock;
			frameTable[node->frame]->dirtyBit=1;
			return 0;
		}else if(node->next==NULL){									//If not found at the end, add it same as the first case
			faults++;
			node->next=malloc(sizeof(list));
			node->next->pageNum=pageNumber;
			node->next->next=NULL;
			node->next->frame=addToFrameTable(pageNumber,hashTable,frameTable,tableSize,clock,isWrite,algorithm);
			return 0;
		}else{
			node=node->next;
		}
	}
}	
int addToFrameTable(int pageNumber,table* hashTable,memory** frameTable,int tableSize,int clock,int isWrite,int algorithm){
	int i,victimizePoint;
	for(i=0;i<tableSize;i++){
		if(frameTable[i]==NULL)
		break;
	}
	if(i<tableSize){									//If the table is not full, add it in the first empty space
		frameTable[i]=malloc(sizeof(memory));
		frameTable[i]->clock=clock;
		frameTable[i]->isWrite=isWrite;
		frameTable[i]->pageNum=pageNumber;
		frameTable[i]->proccessId=hashTable;
		frameTable[i]->dirtyBit=1;
		reads++;
		return i;
	}else{												//Else, select a page to victimize, based on the selected algorithm
		victimizePoint=victimize(frameTable,tableSize,algorithm);
		frameTable[victimizePoint]->clock=clock;
		frameTable[victimizePoint]->isWrite=isWrite;
		frameTable[victimizePoint]->pageNum=pageNumber;
		frameTable[victimizePoint]->proccessId=hashTable;
		frameTable[victimizePoint]->dirtyBit=1;
		reads++;
		return victimizePoint;
	}
}
struct table* initTable(int size,int id){			//Initialize the Table
	table* hashTable=malloc(sizeof(table));
	hashTable->buckets=malloc(sizeof(list*)*size);
	hashTable->size=size;
	hashTable->id=id;
	return hashTable;
}
int victimize(memory** frameTable,int size,int algorithm){
	int min=100000000,posMin,flag=0;
	if(algorithm==0){							//If we have LRU, select the oldest page, meaning the one with the minimum clock value
		for(int i=0;i<size;i++){
			if(frameTable[i]->clock<min){
				min=frameTable[i]->clock;
				posMin=i;
			}
		}
	}else{										//If we have Second Chance, cycle until we find a page with no more chances
		while(flag==0){
			for(int i=0;i<size;i++){
				if(frameTable[i]->dirtyBit==1){
					frameTable[i]->dirtyBit=0;
				}else{
					posMin=i;
					flag=1;
					break;
				}
			}	
		}
		
	}											//At this point, we have selected the page with one of the algorithms
	if(frameTable[posMin]->isWrite==1){			//If it was on write, increase write counter
			writes++;
		}
		deleteFromHashTable(frameTable[posMin]->pageNum,frameTable[posMin]->proccessId);		//Then we remove it from its hash Table
		return posMin;																			//And return the -now empty- frame table index
}
int deleteFromHashTable(int pageNum,table* proccessId){
	list* curr,*prev;
	if(proccessId->buckets[hash(pageNum,proccessId->size)]->pageNum==pageNum){			//If the page is the head of the list
		curr=proccessId->buckets[hash(pageNum,proccessId->size)];						//Delete and make the second one the new head
		proccessId->buckets[hash(pageNum,proccessId->size)]=curr->next;
		free(curr);
		return 0;
	}else{																	//Else, we search in the list and remove 
		curr=proccessId->buckets[hash(pageNum,proccessId->size)];
		while(curr!=NULL){
			prev=curr;
			curr=curr->next;
			if(curr->pageNum==pageNum){
				prev->next=curr->next;
				free(curr);
				curr=curr->next;
				return 0;
			}
		}
	}
}
int hash(int pageNum,int tableSize){		//Simple Hashing Algorithm
	return pageNum%tableSize;
}
