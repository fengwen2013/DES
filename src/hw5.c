#include "hw5.h"

int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "Error: Too few arguments\n");
		return 0;
	}
	
	if(strcmp(argv[1], "tablecheck") == 0){
		checkCommand(argv, argc);
	}
	else{
		if(strcmp(argv[1], "encrypt") == 0){
			cryptCommand(argv, argc, DES_MONO, ENCRYPT);
		}
		else{
			if(strcmp(argv[1], "decrypt") == 0){
				cryptCommand(argv, argc, DES_MONO, DECRYPT);
			}
			else{
				if(strcmp(argv[1], "encrypt3") == 0){
					cryptCommand(argv, argc, DES_EDE, ENCRYPT);
				}
				else{
					if(strcmp(argv[1], "decrypt3") == 0){
						cryptCommand(argv, argc, DES_EDE, DECRYPT);
					}
					else{
						fprintf(stderr, "Error: First argument should be (tablecheck | encrypt | decrypt | encrypt3 | decrypt3)!!!\n");
						return 0;
					}
				}
			}
		}
	}
	return 0;
}
	

void checkCommand(char *argv[], int argc){
	FILE *ifp = stdin;
	if(argc != 3){
		fprintf(stderr, "Error: Wrong number of arguments for tablecheck command\n");
		return;
	}
	else{
		if(optionCheck(argv[2], "-t=") == -1){
				return;
		}
		else{
			if((ifp = openFile(argv[2] + 3)) == NULL){
				return;	
			}
			tablecheck(ifp);
		}
	}
}

void cryptCommand(char *argv[], int argc, int mode, int func){
	FILE *ifp = stdin;
	FILE *tfp = NULL;
	char *key = NULL;
	int m = 0;

	if(argc < 4 || argc > 5){
		fprintf(stderr, "Error: Wrong number of arguments for the command\n");
		return;
	}
	else{
		if(optionCheck(argv[2], "-k=") == -1){
				return;
		}
		else{
			if(mode == DES_MONO){
				key = (char *)malloc(sizeof(char) * 8);	
			}
			else{
				key = (char *)malloc(sizeof(char) * 24);
			}
			if((keycheck(argv[2] + 3, key, mode)) == -1){
				return;
			}
		}
	
		if(optionCheck(argv[3], "-t=") == -1){
				return;
		}
		else{
			if((tfp = openFile(argv[3] + 3)) == NULL){
				return;
			}
			if(tablecheck(tfp) == -1){
				return;
			}
		}
		
		if(argc == 5){
			if((ifp = openFile(argv[4])) == NULL){
				return;	
			}
		}
		
		switch((m = func + mode)){
			case DES_MONO + ENCRYPT:
				encrypt(key, tfp, ifp);
				break;
			case DES_MONO + DECRYPT:
				decrypt(key, tfp, ifp);
				break;
			case DES_EDE + ENCRYPT:
				encryptEDE(key, tfp, ifp);
				break;
			case DES_EDE + DECRYPT:
				decryptEDE(key, tfp, ifp);
				break;	
		}
	}
}

int hexcheck(char c, char *c1){
	if(c >= '0' && c <= '9'){
		*c1 = c - '0';
	}
	else{
		if( c >= 'a' && c <= 'f'){
			*c1 = c - 'a' + 0xa;
		}
		else{
			fprintf(stdout, "Error: '%c' is not valid hex character!\n", c);
			return 0;
		}
	}
	return 1;
}

int keycheck(char *src, char *key, int mode){
	int len = 16;
	int i = 0;
	char high = 0;
	char low = 0;
	if(mode == DES_EDE){
		len = 48;
	}
	
	if(strlen(src) != len){
		fprintf(stderr, "Error: The key length should be %d bytes\n", len / 2);
		return -1;
	}
	
	for(; i < len; i=i+2){
		if(hexcheck(src[i], &high) && hexcheck(src[i+1], &low)){
			key[i/2] = (high << 4) + low;
		}
		else{
			return -1;
		}
		
	}
	return 1;
}
