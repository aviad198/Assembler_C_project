
#include "functionsForAssembler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#define TOTAL_OP 16

int IC =100;
int DC =0;

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