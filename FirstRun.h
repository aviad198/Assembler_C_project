
#include "functionsForAssembler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#define TOTAL_OP 16



extern FILE *fp;


//char function[5];
char label[50];

bool isLabel(char param[50]);

bool checkComma();

void get_opcode(char *opcode);

void addSign(char label[50], char character[5], int value);

void addData();



int validate_command(char *opcode);


void fillin_op_table();

void getVar();

void addString();



bool labelFlag = 0;



void get_opcode(char *opcode);

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



op opTable[16];