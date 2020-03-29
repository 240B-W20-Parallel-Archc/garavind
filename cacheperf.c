#include "cacheperf.h"
#include <stdio.h>

int ThreadCount;
int ArraySize;
bool FlagRandom;
bool FlagSequential;
bool FlagRMW;
int Select;
double ArraySize_kb;


struct LL {
	int item;
	struct LL* NextPointer;
};

void LL_Random(struct LL **List) {
	int Length = (ArraySize)/(sizeof(int));
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < Length - 1; i++) {
				int Swap = rand() % (Length - 1);
				struct LL* Temp;
				Temp = List[i]->NextPointer;
				List[i]->NextPointer = List[Swap]->NextPointer;
				List[Swap]->NextPointer = Temp;
			}
	}
	List[Length-1]->NextPointer = List[0];
}

void LL_Sequential(struct LL **List, int Select) {
	int Length = (ArraySize)/(sizeof(int));
	for (int i = 0; i < Length - 1; i++) {
			List[i]->NextPointer = List[i+Select-1]->NextPointer;
	}
	List[Length-1]->NextPointer = List[0];
}


void Performance() {

	FILE *File;
	File = fopen("performance_output.csv", "a+");

	int Length = (ArraySize)/(sizeof(int));
	struct LL* MemoryAllocated = (struct LL*)malloc(Length*sizeof(struct LL));
	struct LL **List = (struct LL**)malloc(Length*sizeof(struct LL));
	for (int i = 0; i < Length; ++i) {
		List[i] = &MemoryAllocated[i];
	}

	for(int i = 0; i < Length - 1; i++) {
		List[i]->item = i;
		List[i]->NextPointer = List[i+1];
	}
	List[Length-1]->item=Length-1;
	List[Length-1]->NextPointer=List[0];


	if(FlagRandom) {
		LL_Random(List);
	} else {
		LL_Sequential(List, Select);
	}

	if(!FlagRMW) {
		clock_t StartTime, EndTime;
		double CPUTime;
		int ReadCount = 0;
		int LoopCount = Length;
		StartTime = clock();
		struct LL* NextNode = List[1];
		while (LoopCount > 0) {
			NextNode = NextNode->NextPointer;
			ReadCount++;
			LoopCount--;
			}
		EndTime = clock();
		CPUTime = (1e9 * ( (double)(EndTime - StartTime)/CLOCKS_PER_SEC)) / ReadCount;
		fprintf(File,"%lf,%d,%d,%d,%d,%lf\n", ArraySize_kb,ThreadCount,FlagRandom,FlagSequential,FlagRMW,CPUTime);
		}

	else {
		clock_t StartTime_RMW, EndTime_RMW;
		double CPUTime_RMW;
		int RMWCount = 0;
		int LoopCount_RMW = Length;
		StartTime_RMW = clock();
		struct LL* NextNode_RMW = List[1];
		uint64_t s;
		while (LoopCount_RMW > 0) {
			s = NextNode_RMW->item;
			s = s ^ 0x7FFFFEEF;
			NextNode_RMW->item = s;
			NextNode_RMW = NextNode_RMW->NextPointer;
			RMWCount++;
			LoopCount_RMW--;
		}
		EndTime_RMW = clock();
		CPUTime_RMW = (1e9 * ( (double)(EndTime_RMW - StartTime_RMW)/CLOCKS_PER_SEC)) / RMWCount;
		fprintf(File,"%lf,%d,%d,%d,%d,%lf\n", ArraySize_kb,ThreadCount,FlagRandom,FlagSequential,FlagRMW,CPUTime_RMW);
	}
	fclose(File);
}
