#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

#define DES_MONO 0
#define DES_EDE	 1
#define ENCRYPT  2
#define DECRYPT  4

void encryptEDE(char key[], FILE *tfp, FILE *ifp);
void decryptEDE(char key[], FILE *tfp, FILE *ifp);
void encrypt(char key[], FILE *tfp, FILE *ifp);
void decrypt(char key[], FILE *tfp, FILE *ifp);
int tablecheck(FILE *ifp);

void checkCommand(char *argv[], int argc);
void cryptCommand(char *argv[], int argc, int mode, int func);

FILE* openFile(char *FileName);
int optionCheck(char *option, char *check);
int keycheck(char *src, char *key, int mode);
