#include "hw5.h"

char *substr(char *s, int position, int length){
	char *p = malloc(length + 1);
	int i = 0;
	
	if(p == NULL){
		fprintf(stderr, "Unable to allocate memory!\n");
		return NULL;
	}
	
	while(i < position){
		s++;
		i++;
	}
	
	for(i = 0; i < length; i++){
		*(p + i) = *(s++);
	}
	
	*(p + i) = '\0';
	
	return p;
}

void initArray(int a[], int len, int value){
	int i = 0;
	for(; i < len; i++){
		a[i] = value;
	}
}

int checkP(int T[], int len, int max, FILE *ifp, char *tablename){
	char sT[256];
	int buffT[max];
	char *number = NULL;
	int num = 0;
	int i = 0;
	char s[10] = "";
	
	fgets(sT, 256, ifp);
	strcat(s, tablename);
	strcat(s, "=");
	if(strcmp(substr(sT, 0, strlen(s)), s) != 0){
		fprintf(stderr, "Error: Malformed table file, should start with \"%s\"\n", s);
		return -1;
	}
	
	initArray(buffT, max, -1);
	
	number = strtok(sT + strlen(s), ", ");
	while(number != NULL && i < len){
		sscanf(number, "%d", &num);
		if(num < 1 || num > max){
			fprintf(stderr, "Error: Value %d is not allowed\n in %s Table", num, tablename);
			return -1;
		}
		
		if(buffT[num - 1] == -1){
			buffT[num - 1] = i;
		}
		else{
			fprintf(stderr, "Error: Value %d should not more than once! in %s\n", num, tablename);
			return -1;
		}
		i++;
		number = strtok(NULL, ", ");
	}
	
	if(i != len){
		fprintf(stderr, "Error: Too few values read in %s!\n", tablename);
		return -1;
	}
	
	if(number != NULL){
		fprintf(stderr, "Error: Too many values read in %s!\n", tablename);
		return -1;
	}
	
	for(i = 0; i < max; i++){
		if(buffT[i] == -1){
			if((i + 1) % 8 != 0 && strcmp(tablename, "PC1") == 0){
				fprintf(stderr, "Error: %d doesn't occur in %s!\n", i + 1, tablename);
				return -1;
			}
		}
		if(buffT[i] != -1){
			T[buffT[i]] = i;
		}
	}
	
	
	return 1;

}

int checkE(int E[], int len, FILE *ifp){
	char sE[256];
	int buffE[32];
	char *number = NULL;
	int num = 0;
	int i = 0;
	
	fgets(sE, 256, ifp);
	if(strcmp(substr(sE, 0, 2), "E=") != 0){
		fprintf(stderr, "Error: Malformed table file, should start with \"E=\"\n");
		return -1;
	}
	
	initArray(buffE, 32, 0);
	
	number = strtok(sE + 2, ", ");
	while(number != NULL && i < len){
		sscanf(number, "%d", &num);
		if(num < 1 || num > 32){
			fprintf(stderr, "Error: Value %d is not allowed\n in E Table", num);
			return -1;
		}
		buffE[num - 1] ++;
		E[i] = num - 1;
		i++;
		number = strtok(NULL, ", ");
	}
	
	if(i != len){
		fprintf(stderr, "Error: Too few values read in E!\n");
		return -1;
	}
	
	if(number != NULL){
		fprintf(stderr, "Error: Too many values read in E!\n");
		return -1;
	}
	
	for(i = 0, num = 0; i < 32; i++){
		if(buffE[i] == 0){
			fprintf(stderr, "Error: Number %d in E doesn't occur\n", i + 1);
			return -1;
		}
		else{
			if(buffE[i] > 2){
				fprintf(stderr, "Error: Number %d in E occurs too many times\n", i + 1);
				return -1;
			}
			else{
				num += buffE[i];
			}
		}
	}
	
	
	return 1;
}



int checkSi(int Si[], int len, FILE *ifp, char *head){
	char sSi[256];
	int buffSi[16];
	char *number = NULL;
	int num = 0;
	int i = 0;
	
	fgets(sSi, 256, ifp);
	if(strcmp(substr(sSi, 0, strlen(head)), head) != 0){
		fprintf(stderr, "Error: Malformed table file, should start with \"%s\"\n", head);
		return -1;
	}
	
	initArray(buffSi, 16, 0);
	
	number = strtok(sSi + 3, ", ");
	while(number != NULL && i < len){
		sscanf(number, "%d", &num);
		if(num >= 0 && num <= 15){
			buffSi[num] ++;
			Si[i] = num;
			
		}
		
		i++;
		number = strtok(NULL, ", ");
	}
	
	if(i != len){
		fprintf(stderr, "Error: Too few values read in %c%c!\n", head[0], head[1]);
		return -1;
	}
	
	if(number != NULL){
		fprintf(stderr, "Error: Too many values read in %c%c!\n", head[0], head[1]);
		return -1;
	}
	
	for(i = 0, num = 0; i < 16; i++){
		if(buffSi[i] != 4){
			fprintf(stderr, "Error: Number %d in %c%c should occur 4 times\n", i, head[0], head[1]);
			return -1;
		}
		else{
			num += buffSi[i];
		}		
	}
	
	
	return 1;
}

int checkSTable(int S[8][64], FILE *ifp){
	int i = 0;
	char head[] = "S1=";
	for(; i < 8; i++){
		head[1] = i + '1';
		if(checkSi(S[i], 64, ifp, head) == -1){
			return -1;
		}
	}
	
	return 1;
}

int checkV(int V[], int len, FILE *ifp){
	char sV[256];
	int buffV[2];
	char *number = NULL;
	int num = 0;
	int i = 0;
	
	fgets(sV, 256, ifp);
	if(strcmp(substr(sV, 0, 2), "V=") != 0){
		fprintf(stderr, "Error: Malformed table file, should start with \"E=\"\n");
		return -1;
	}
	
	initArray(buffV, 2, 0);
	
	number = strtok(sV + 2, ", ");
	while(number != NULL && i < len){
		sscanf(number, "%d", &num);
		if(num >= 1 && num <= 2){
			buffV[num - 1] ++;
			V[i] = num;
			
		}
		i++;
		number = strtok(NULL, ", ");
		
	}
	
	if(i != len){
		fprintf(stderr, "Error: too few values read in V!\n");
		return -1;
	}
	
	if(number != NULL){
		fprintf(stderr, "Error: too many values read in V!\n");
		return -1;
	}
	
	if(buffV[0] != 4){
		fprintf(stderr, "Error: The value 1 in the V doesn't occur 4 times\n");
		return -1;	
	}
	
	if(buffV[1] != 12){
		fprintf(stderr, "Error: The value 2 in the V doesn't occur 12 times\n");
		return -1;
	}
	
	
	
	return 1;
}

int tablecheck(FILE *ifp){
	int IP[64];
	int E[48];
	int P[32];
	int S[8][64];
	int V[16];
	int PC1[56];
	int PC2[48];
	int i = 0;
	int j = 0;
	
	if(checkP(IP, 64, 64, ifp, "IP") == -1){
		return -1;
	}
	
	if(checkE(E, 48, ifp) == -1){
		return -1;
	}
	
	if(checkP(P, 32, 32, ifp, "P") == -1){
		return -1;
	}
	
	
	
	if(checkSTable(S, ifp) == -1){
		return -1;
	}
	
	
	
	if(checkV(V, 16, ifp) == -1){
		return -1;
	}
	
	
	
	if(checkP(PC1, 56, 64, ifp, "PC1") == -1){
		return -1;
	}
	
	if(checkP(PC2, 48, 56, ifp, "PC2") == -1){
		return -1;
	}	
	/*
	*/
	
	fclose(ifp);
	return 1;
}
