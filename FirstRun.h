
#include "functionsForAssembler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#define TOTAL_OP 16


int opCode[TOTAL_OP] = {0, 1, 2, 2, 4, 5, 5, 5, 5, 9, 9, 9, 12, 13, 14, 15};
char allOp[TOTAL_OP][4] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red",
                           "prn", "rts", "stop"};
int func[TOTAL_OP] = {0, 0, 1, 2, 0, 1, 2, 3, 4, 1, 2, 3, 0, 0, 0, 0};
int sourceMeth[TOTAL_OP][4] = {{0,  1, -1, 3},
                               {0,  1, -1, 3},
                               {0,  1, -1, 3},
                               {0,  1, -1, 3},
                               {-1, 1, -1, -1},
                               {-1},
                               {-1},
                               {-1},
                               {-1},
                               {-1},
                               {-1},
                               {-1},
                               {-1},
                               {-1},
                               {-1},
                               {-1}};
int targMeth[TOTAL_OP][4] = {{-1, 1, -1, 3},
                             {0,  1, -1, 3},
                             {-1, 1, -1, 3},
                             {-1, 1, -1, 3},
                             {-1, 1, -1, 3},
                             {-1, 1, -1, 3},
                             {-1, 1, -1, 3},
                             {-1, 1, -1, 3},
                             {-1, 1, -1, 3},
                             {-1, 1, 2,  -1},
                             {-1, 1, 2,  -1},
                             {-1, 1, 2,  -1},
                             {-1, 1, -1, 3},
                             {0,  1, -1, 3},
                             {-1},
                             {-1}};
int oprndN[TOTAL_OP] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
int lineCounter;
int IC;
int DC;

extern FILE *fp;


//char function[5];
char label[50];

bool isLabel(char param[50]);

bool checkComma();

void getCommand(char *command);

void addSign(char label[50], char character[5], int value);

void addData();



int validateCommand(char *command);


void fillinOpTable();

void getVar();

void addString();

symboleTabel *curSNode;
dataNode *curDNode;
bool labelFlag = 0;



void getCommand(char *command);

/*struc for operatons table*/
typedef struct{
    int opcode;
    int func;
    char *opName;
    int *sourceMeth;
    int *targMeth;
    int oprndN;
}op;



bool insertData();

int getAdress(char label[200]);
op opTable[16];