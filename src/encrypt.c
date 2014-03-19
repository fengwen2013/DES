#include "hw5.h"

void kengen(uint64_t *k, unsigned char key[]){
	int i = 0;
	for(i = 0; i < 8; i++){
		(*k) = (*k) << 8;
		(*k) += key[i];
	}
}

void initBytes(unsigned char T[], int len, int value){
	int i = 0;
	for(; i < len; i++){
		T[i] = value;
	}
}
void keySchedule(unsigned char key[], char subkeys[16][48]){
	uint64_t k = 0;
	unsigned char T[7];
	int i = 0;
	kengen(&k, key);
	
	initBytes(T, 7, 0);
	
	for(i = 0; i < 56; i++){
		T[i / 8] = T[i / 8] << 1;
		T[i / 8] += ((k >> (63 - PC1[i])) & 0x1);
	}
	
	for(i = 0; i < 7; i++){
		fprintf(stderr, "%02x ", T[i]);
	}
	
	putchar('\n');
}
void encryptEDE(unsigned char key[], FILE *tfp, FILE *ifp){
	printf("encrypt\n");
}

void encrypt(unsigned char key[], FILE *tfp, FILE *ifp){
	char buff[8];
	char subkeys[16][48];
	
	keySchedule(key, subkeys);
	
}

