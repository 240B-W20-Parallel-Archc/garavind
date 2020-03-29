#include "cacheperf.h"


FILE *stream;
FILE *File;
int ThreadCount = 1;
int ArraySize = 1024;
bool FlagSequential = false;
bool FlagRandom = false;
bool FlagRMW = false;
int Select = 1;
double ArraySize_kb = 1.0;

void cacheperf_options() {
	fprintf(stderr, "Cache Performance Project \n");
	fprintf(stderr, "Options: \n");
	fprintf(stderr, "	-t:<number of threads> \n");
	fprintf(stderr, "	-s/-r (sequential or random access) \n");
	fprintf(stderr, "	-size:<Array Size in kb> \n");
	fprintf(stderr, " 	-stride:<stride length> (only for sequential access) \n");
	fprintf(stderr, " 	-RMW (For changing to read-modify-write mode, default is read) \n");
}

int scanning_inputs(char *arg) {
	if(!(strncmp(arg,"-t:",3))) {
		sscanf(arg+3,"%d", &ThreadCount);
	} else if (!strncmp(arg,"-size:",6)) {
		sscanf(arg+6, "%lf", &ArraySize_kb);
		ArraySize = (int)(1024*ArraySize_kb);
	} else if (!strncmp(arg,"-s",2)) {
		FlagSequential = true;
	} else if (!strncmp(arg,"-r",2)) {
		FlagRandom = true;
	} else if (!strncmp(arg, "-RMW",6)) {
		FlagRMW = true;
	} else if (!strncmp(arg, "-stride:", 8)){
		sscanf(arg+8,"%d",&Select);
	} else {
		return 0;
	}
	return 1;
}


int main(int argc, char *argv[]) {
	//Setting defaults
	stream = stdin;

	for (int i = 1; i < argc; i++) {
		if(!(strcmp(argv[i],"--help"))) {
			cacheperf_options();
			//return 0;
		} else if (!strncmp(argv[i],"-",1)){
			scanning_inputs(argv[i]);
			if(!scanning_inputs(argv[i])) {
				fprintf(stderr, "ERROR: Unrecognized options!\n");
				fprintf(stderr, "Refer to the following:\n");
				cacheperf_options();
				exit(0);
			}
		} else {
			// forgot about the case when there is no - in input
			fprintf(stderr, "beepbop\n");
			fprintf(stderr, "ERROR: Unrecognized options!\n");
			fprintf(stderr, "Refer to the following:\n");
			cacheperf_options();
			exit(0);
		}
	}


	if(ThreadCount > 1) {
		pthread_t* threads = (pthread_t*)malloc(ThreadCount*sizeof(pthread_t));
		for (int i = 0; i < ThreadCount; i++) {
			pthread_create(threads + i, NULL, Performance, NULL);
		}
		for (int i = 0; i < ThreadCount; i++) {
			pthread_join(*(threads + i), NULL);
		}
		free(threads);
	} else {

	Performance();
	}
	return 0;
}
