
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_LENGTH 200


char inputOrg[MAX_LENGTH];
char label[50];
char line[MAX_LENGTH];
char *p;
char param[MAX_LENGTH];

bool isLabel(char param1[50]);
void pointAfterParam();
void getParam();
void skipWhite();
int isGuide(char *param);
void int2bin(int integer, char* binary, int n);
bool isNum (char * param);
void addSign(char label[50], char character[50], int value);

typedef struct{
    char label[50];
    int value;
    char car[50];
}Sign;

typedef struct SNode {
    Sign sign;
    struct SNode * next;
}symboleTabel;

symboleTabel* sHead;
symboleTabel* curSNode;

typedef union Data{
    unsigned num : 24;
    char ch;
}Data;

typedef struct DNode {
    Data data;
    struct DNode* next;
}dataNode;


dataNode* dHead;
dataNode* curDNode;


typedef struct{
    int adress;
    int ICval;
    char binCode[25];
    char opName[MAX_LENGTH];
    char opLabel[MAX_LENGTH];
    char operandLabel[MAX_LENGTH];

}Code;

typedef struct CNode {
    Code code;
    struct CNode * next;
}CNode;

CNode* Chead ;
CNode *curCNode;


void firstRun();
void secondRun();
/*void secondRun();*/

bool goodFile;
bool hasExtern;
bool hasEntry;

int IC;
int DC;

symboleTabel * inLabelTab (char param[50]);