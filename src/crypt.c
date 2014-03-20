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

long long unsigned permutation(int P[], 
							long long unsigned src, int len, int max){
	int i = 0;
	long long unsigned k = 0;
	for(i = 0; i < len; i++){
		k = k << 1;
		k += (src >> (max - 1 - P[i])) & 0x1;	
	}
	return k;
}

long long unsigned keyPC2(long unsigned C, long unsigned D){
	long long unsigned T = C;
	T= T << 28 | D;
	long long unsigned k = 0;
	k = permutation(PC2, T, 48, 56);
	
	return k;
}

long unsigned cirLeftShift(long unsigned Ci, int offset){
	return (Ci << offset | Ci >> (28 -offset)) & 0xfffffff;
}

void subkeysGen(long long unsigned subkeys[], 
				long unsigned C, long unsigned D){
	int i = 0;
	long unsigned Ci = 0;
	long unsigned Di = 0;
	for(i = 0, Ci = C, Di = D; i < 16; i++){
		Ci = cirLeftShift(Ci, V[i]);
		Di = cirLeftShift(Di, V[i]);
		fprintf(stderr, "(C%d,D%d)=%07lx%07lx\n", i + 1, i + 1, Ci, Di);
		subkeys[i] = keyPC2(Ci, Di);
		fprintf(stderr, "k%d=%012llx\n", i + 1, subkeys[i]);	
	}
}

void keySchedule(unsigned char key[], long long unsigned subkeys[]){
	uint64_t k = 0;
	long long unsigned T = 0;
	long unsigned C = 0;
	long unsigned D = 0;
	kengen(&k, key);
	
	T = permutation(PC1, k, 56, 64);
	
	C = T >> 28;
	D = T & 0xfffffff;
	fprintf(stderr, "(C0,D0)=%07lx%07lx\n", C, D);
	subkeysGen(subkeys, C, D);
	
}


void printfCT(long long unsigned cipherText, FILE *ofp){
	int i = 0;
	char c = 0;
	for(i = 0; i < 8; i++){
		c = (cipherText >> (7 - i) * 8) & 0xff;
		fprintf(ofp, "%c", c);
	}
}

void inversion(int IP[], int P[], int n){
	int i = 0;
	for(i = 0; i < n; i++){
		P[IP[i]] = i;
	}
}

long unsigned computeRi(long unsigned Li, long unsigned Ri, long long unsigned key){
	long long unsigned y = 0;
	long unsigned int sy = 0;
	int row = 0, col = 0;
	int i = 0;
	int si = 0;
	y = permutation(E, Ri, 48, 32);
	y = y ^ key;
	
	for(i = 0; i < 8; i++){
		sy = sy << 4;
		si = y >> ((7 - i) * 6) & 0x3f;
		col = (si >> 1) & 0xf; 
		row = ((si >> 5) & 0x1) * 2 + (si & 0x1); 
		sy += S[i][row * 16 + col];
	}
	
	y = (Li ^ permutation(P, sy, 32, 32));
	return y;
}
void feistelCipher(long long unsigned message, long long unsigned subkeys[], int flag, FILE *ofp){
	long unsigned Li = 0, Ri = 0;
	long long unsigned cipherText = 0;
	int P[64];
	int i = 0;
	long unsigned buf = 0;
	message = permutation(IP, message, 64, 64);
	Li = (message >> 32);
	Ri = message & 0xffffffff;
	if(flag){
		fprintf(stderr, "(L0,R0)=%08lx%08lx\n", Li, Ri);
	}
	for(i = 1; i <= 16; i++){
		buf = Ri;
		Ri = computeRi(Li, Ri, subkeys[i - 1]);
		Li = buf;
		if(flag){
			fprintf(stderr, "(L%d,R%d)=%08lx%08lx\n", i, i, Li, Ri);
		}
		
	}
	
	inversion(IP, P, 64);
	cipherText = Ri;
	cipherText = Li + (cipherText << 32);
	cipherText = permutation(P, cipherText, 64, 64);
	printfCT(cipherText, ofp);
}

void crypt(unsigned char key[], FILE *ifp, int mode, FILE *ofp){
	long long unsigned message = 0;
	unsigned char buff[8];
	long long unsigned subkeys[16];
	int len = 0;
	int i = 0;
	int flag = 1;
	keySchedule(key, subkeys);
	if(mode == MODE_DE){
		for(i = 0; i < 8; i++){
			message = subkeys[i];
			subkeys[i] = subkeys[15 - i];
			subkeys[15 - i] = message;
		}
	}
	while(!feof(ifp)){
		initBytes(buff, 8, 0);
		len = fread(buff, sizeof(char), 8, ifp);
		if(!feof(ifp) || len != 0){	
			for(i = 0, message = 0; i < 8; i++){
				message = message << 8;
				message += buff[i];
			}
			feistelCipher(message, subkeys, flag, ofp);
			flag = 0;
		}
	}
}

void encryptEDE(unsigned char key[], FILE *ifp, int mode){
	FILE *ofp1 = NULL;
	FILE *ofp2 = NULL;
	unsigned char levelKey1[8];
	int i = 0;
	//E
	for(i = 0; i < 8; i++){
		levelKey1[i] = key[i];
	}
	ofp1 = fopen("1.tem", "w");
	crypt(levelKey1, ifp, mode, ofp1);
	//D
	for(i = 8; i < 16; i++){
		levelKey1[i % 8] = key[i];
	}
	fclose(ofp1);
	ofp1 = fopen("1.tem", "r");
	ofp2 = fopen("2.tem", "w+");
	mode = mode ^ 1;
	crypt(levelKey1, ofp1, mode, ofp2);
	//E
	for(i = 16; i < 24; i++){
		levelKey1[i % 8] = key[i];
	}
	fclose(ofp2);
	ofp2 = fopen("2.tem", "r");
	mode = mode ^ 1;
	crypt(levelKey1, ofp2, mode, stdout);
	
	fclose(ofp1);
	fclose(ofp2);
	remove("1.tem");
	remove("2.tem");
}

void decryptEDE(unsigned char key[], FILE *ifp, int mode){
	FILE *ofp1 = NULL;
	FILE *ofp2 = NULL;
	unsigned char levelKey1[8];
	int i = 0;
	//E
	for(i = 16; i < 24; i++){
		levelKey1[i % 8] = key[i];
	}
	ofp1 = fopen("1.tem", "w");
	crypt(levelKey1, ifp, mode, ofp1);
	//D
	for(i = 8; i < 16; i++){
		levelKey1[i % 8] = key[i];
	}
	fclose(ofp1);
	ofp1 = fopen("1.tem", "r");
	ofp2 = fopen("2.tem", "w+");
	mode = mode ^ 1;
	crypt(levelKey1, ofp1, mode, ofp2);
	//E
	for(i = 0; i < 8; i++){
		levelKey1[i % 8] = key[i];
	}
	fclose(ofp2);
	ofp2 = fopen("2.tem", "r");
	mode = mode ^ 1;
	crypt(levelKey1, ofp2, mode, stdout);
	
	fclose(ofp1);
	fclose(ofp2);
	remove("1.tem");
	remove("2.tem");
}




