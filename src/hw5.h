#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "stdint.h"

#define DES_MONO 0
#define DES_EDE	 1
#define ENCRYPT  2
#define DECRYPT  4
#define MODE_EN  0
#define MODE_DE  1

void encryptEDE(unsigned char key[], FILE *ifp, int mode);
void decryptEDE(unsigned char key[], FILE *ifp, int mode);
void crypt(unsigned char key[], FILE *ifp, int mode, FILE *ofp);
int tablecheck(FILE *ifp);

void checkCommand(char *argv[], int argc);
void cryptCommand(char *argv[], int argc, int mode, int func);

FILE* openFile(char *FileName);
int optionCheck(char *option, char *check);
int keycheck(char *src, unsigned char *key, int mode);

extern int IP[64];
extern int E[48];
extern int P[32];
extern int S[8][64];
extern int V[16];
extern int PC1[56];
extern int PC2[48];
