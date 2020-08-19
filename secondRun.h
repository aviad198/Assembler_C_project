
#include "functionsForAssembler.h"
void addLabelOperand();
void printObjFile();
extern FILE *fp;
char *p;
char line[MAX_LENGTH];

FILE * objFile;
FILE * extFile;
FILE * entFile;