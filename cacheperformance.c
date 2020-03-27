#include "cacheperformance.h"
#include <stdio.h>

int threadcnt;
int array_size;
bool random_flag;
bool sequential_flag;
bool write_flag;
int inc;
double size_kb;


struct Node {
	int data;
	struct Node* nextptr;
};

void random_ll(struct Node **lst) {
	int length = (array_size)/(sizeof(int));
	for (int i = 0; i < 5; i++) {
		for (int i = 0; i < length - 1; i++) {
				int idx = rand() % (length - 1);
				struct Node* temp;
				temp = lst[i]->nextptr;
				lst[i]->nextptr = lst[idx]->nextptr;
				lst[idx]->nextptr = temp;
			}
	}
	lst[len-1]->nextptr = lst[0];
}

void sequential_ll(struct Node **lst, int inc) {
	int length = (array_size)/(sizeof(int));
	for (int i = 0; i < length - 1; i++) {
			lst[i]->nextptr = lst[i+inc-1]->nextptr;
	}
	lst[len-1]->nextptr = lst[0];
}

void latency() {

	FILE *file;
	file = fopen("output.csv", "a+");

	int length = (array_size)/(sizeof(int));
	struct Node* mem = (struct Node*)malloc(len*sizeof(struct Node));
	struct Node **lst = (struct Node**)malloc(len*sizeof(struct Node));
	for (int i = 0; i < length; ++i) {
		lst[i] = &mem[i];
	}

	for(int i = 0; i < length - 1; i++) {
		lst[i]->data = i;
		lst[i]->nextptr = lst[i+1];
	}
	lst[len-1]->data=len-1;
	lst[len-1]->nextptr=lst[0];


	if(random_flag) {
		//printf("Moving into random loop\n");
		random_ll(lst);
	} else {
		sequential_ll(lst, inc);
	}

	if(!write_flag) {
		//printf("Only read case loop\n");
		clock_t start_time, end_time;
		double cputime_ns;
		int r_cnt = 0;
		int l_cnt = length;
		start_time = clock();
		struct Node* nextnode = lst[1];
		while (l_cnt > 0) {
			nextnode = nextnode->nextptr;
			r_cnt++;
			l_cnt--;
			}
		end_time = clock();
		cputime_ns = (1e9 * ( (double)(end_time - start_time)/CLOCKS_PER_SEC)) / r_cnt;
		fprintf(fp,"%lf,%d,%d,%d,%d,%lf\n", size_kb,threadcnt,random_flag,sequential_flag,write_flag,cputime_ns);
		}

	else {
    		//printf("Read Modify Write (RMW) case loop\n");
		clock_t start_time_rmw, end_time_rmw;
		double cputime_rmw;
		int rmw_count = 0;
		int loop_rmw_count = length;
		start_time_rmw = clock();
		struct Node* nextnode_rmw = lst[1];
		uint64_t swap;
		while (loop_rmw_count > 0) {
			swap = nextnode_rmw->data;
			swap = swap ^ 0x7FFFFEEF;
			nextnode_rmw->data = swap;
			nextnode_rmw = nextnode_rmw->nextptr;
			rmw_count++;
			loop_rmw_count--;
		}
		end_time_rmw = clock();
		cputime_rmw = (1e9 * ( (double)(end_time_rmw - start_time_rmw)/CLOCKS_PER_SEC)) / rmw_count;
		fprintf(fp,"%lf,%d,%d,%d,%d,%lf\n", size_kb,threadcnt,random_flag,sequential_flag,write_flag,cputime_rmw);
	}
	fclose(fp);
}
