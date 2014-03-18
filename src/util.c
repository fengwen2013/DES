#include "hw5.h"

int optionCheck(char *option, char *check){
	int clen = strlen(check);
	int olen = strlen(option); 
	int i = 0;
	
	if(olen <= clen){
		fprintf(stderr, "Error: Wrong option, should be '%soption_value'\n", check);
		return -1;
	}
	while(i < clen){
		if(option[i] != check[i]){
			fprintf(stderr, "Error: Wrong option, should be '%soption_value'\n", check);
			return -1;
		}
		i++;
	}
	return 1;
}

FILE* openFile(char *fileName){
	FILE *ifp = NULL;
	char *mode = "r";
	if(access(fileName, F_OK) == -1){
		fprintf(stderr, "Error: File %s doesn't exsit!\n", fileName);
		return NULL;
	}
	else{
		ifp = fopen(fileName, mode);
		if(ifp == NULL){
			fprintf(stderr, "Error: Can't open %s\n", fileName);
		}
		return ifp;
	}
}

