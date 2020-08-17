
#include "FirstRun.h"

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


/*start first run */
void firstRun() {

    /*new heads for linked lists */
    if(!(sHead = (symboleTabel *) malloc(sizeof(symboleTabel)))){
        printf("Memory allocate failure");
        return;
    }
    curSNode = sHead;
    curSNode->sign.label[0] = '\0';
    if(!(dHead = (dataNode *) malloc(sizeof(dataNode)))){
        printf("Memory allocate failure");
        return;
    }
    curDNode = dHead;
    if(!( Chead = (CNode *) malloc(sizeof(CNode)))){
        printf("Memory allocate failure");
        return;
    }
    curCNode = Chead;
    curCNode->code.binCode[24] = '\0';
    fillin_op_table();
    /*while file has a line*/
    while (fgets(line, MAX_LENGTH, fp)) {
        /*points to first letter in line*/
        p = line;
        if (*p == ';') {
            continue;
        }
        skipWhite();
        getParam();
        /*turn on flag if there is a sign*/
        if (labelFlag = isLabel(param) == 1) {
            int i;
            int length = strlen(label);
            if (inLabelTab(label)) {
                printf("alleardy is label with same name\n");
                continue;
            }
            if (!isalpha(*label)) {//*param < 'A' || (*param > 'Z' && *param < 'a') || *param > 'z')
                printf("label has to start with letter\n");
                continue;
            }
            if (length > 31) {
                printf("label is too long\n");
                continue;
            }
            if (isGuide(label)) { /*need - or command or register or already is a label*/
                printf("unvalid name for label\n");
                continue;
            }
            for (i = 0; i < length; i++) {
                if (!isalnum(*(label + i))) {
                    printf("label has to be contain alpha/num\n");
                    continue;
                }
            }

        }
        /*gets next param*/
        if (labelFlag) {
            getParam();
        }
        /*if its a guide command (.data/.string/.extern/.entry)*/
        switch (isGuide(param)) {
            /*.data guide */
            case 1: {

                if (labelFlag)
                    addSign(label, "data", DC);
                //getParam();
                addData();
            }
                break;
                /*.string guide */
            case 2: {

                if (labelFlag)
                    addSign(label, "data", DC);
                //getParam();
                addString();
            }
                break;
                /*entry guide*/
            case 3: {
            }
                break;
                /*extern guide*/
            case 4: {
                if (labelFlag) {
                    printf("cant put .extern after label");
                    continue;
                }
                getParam();
                if (*param == '\n') {
                    printf("missing extern label");
                    continue;
                }
                addSign(param, "external", 0);
            }
                break;
            default: {
                if (labelFlag) {
                    if (inLabelTab(label))
                        continue;
                    addSign(label, "code", IC);

                }

                get_opcode(param);
            }
                break;
        }
/*go over symbolTabel and add 100 to value*/

    }


}


/*method to add string data into data image
 *
 * needs - a.to check the string
 *         b. check extranous data
 * */
void addString() {
    int check = 0;
    skipWhite();
    /*if nothing to add to data*/
    if (*p == '\n') {
        printf("missing data\n");
        return;
    }
    /*dosnt start string with " */
    if (*p != '\"') {
        printf("has to start with \" \n");
        return;
    }
    p++;
    skipWhite();
    if (!isascii(*p)) { //maybe needs to be changed to isascii
        printf("missing char\n");
        return;
    }
    /*while not end of line or end of string*/
    while (*p != '\0' && *p != '\"') {

        /*adds the char to data linked list*/
        curDNode->data.ch = *p;
        curDNode->next = (dataNode *) malloc(sizeof(dataNode));

        printf("%d: %c\n", check, curDNode->data.ch);

        curDNode = curDNode->next;

        DC++;
        p++;
        skipWhite();
        check++;
    }
    /*check if last char is "*/
    if (*p != '\"') {
        printf("has to end with \" \n");
        return;
    }
    /*adds the char \0 at the end of linked list*/
    curDNode->data.ch = '\0';
    curDNode->next = (dataNode *) malloc(sizeof(dataNode));
    curDNode =  curDNode->next;
    DC++;

    p++;
    skipWhite();
    if (*p != '\0') {
        printf("extra input after .string\n");

        return;
    }

}

int startWithComma() {
    skipWhite();
    /*if starts with comma*/
    if (*p == ',') {
        printf("statement cannot start with a comma\n");
        return 0;
    }
    /*if empty data*/
    if (*p == '\n') {
        printf("missing data\n");
        return 0;
    }
    return 1;
}


/*adds data to data linked list*
 *
 * needs- a. return error if comma after last var*/

void addData() {
    int check = 0;//temporary check
    skipWhite();

    /*if starts with comma*/
    if (*p == ',') {
        printf("bad place for comma\n");
        return;
    }
    /*if empty data*/
    if (*p == '\n') {
        printf("missing data\n");
        return;
    }

    if (!startWithComma())
        return;
    getVar();
    if(!insertData())
        return;

/*while not end of line*/
    while (*p != '\0') {

        /*if missing comma or mult commas*/
        if (!checkComma()) {
            return;

        } else {
            if(!insertData())
                return;
            }

        }

    }



bool insertData() {
    if (!isNum(param)) {
        printf("wrong input for data\n");
        return 0;
    } else {
        /*insert data*/
        curDNode->data.num = atoi(param);
        curDNode->next = (dataNode *) malloc(sizeof(dataNode));
        curDNode = curDNode->next;
        DC++;
        return 1;
    }
}

/*get a var diveded by commas */
void getVar() {
    skipWhite();
    sscanf(p, "%[^,]", param);
    skipWhite();
    pointAfterParam();
}

/*checks if has comma ot not or have multiple commas*/
bool checkComma() {
    /*if got two vars with no comma to sparate between them*/
    char check1[MAX_LENGTH];
    char check2[MAX_LENGTH];
    int counter = 0;
    if (sscanf(param, "%s %s", check1, check2) > 1) {
        printf("Missing comma\n");
        return false;
    }
    skipWhite();


    /*check if the first letter (after skipWhite is a comma and looks for multiple commas*/
    while (*p == ',') {
        counter++;
        p++;

        skipWhite();
    }
    /*if no data after comma*/
    if (*p == '\0') {
        printf("Missing Data after comma\n");
        return false;
    }
    /*missing comma*/
    if (counter == 0) {
        printf("Missing comma\n");
        return false;
        /*more then one comma*/
    } else if (counter > 1) {
        printf("Multiple consecutive commas\n");
        return false;
    }
    return true;

}



//bool validat_command(

void fillin_op_table() {
    int i;
    for (i = 0; i < TOTAL_OP; i++) {
        opTable[i].opName = allOp[i];
        opTable[i].opcode = opCode[i];
        opTable[i].func = func[i];
        opTable[i].sourceMeth = sourceMeth[i];
        opTable[i].targMeth = targMeth[i];
        opTable[i].oprndN = oprndN[i];
    }
}


int validate_command(char *opcode) {
    int i;
    for (i = 0; i < TOTAL_OP; i++) {
        if (!(strcmp(opcode, opTable[i].opName))) {
            printf("the command %s is valid\n", opcode);
            return i;
        }
    }
    return -1;
}

int check_non_white_char(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (!isspace(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}

int check_register_meth3(char *OP) {
    if (OP[0] == 114 && isdigit(OP[1])) {
        if (atoi(OP + 1) < 0 || atoi(OP + 1) > 7)
            return 0;
        if (strlen(OP) > 2 && !check_non_white_char(OP + 2))
            return 0;
    } else
        return 0;
    return 1;
}


int check_label(char *OP) {
    int i;
    int length = strlen(OP);
    if (!isalpha(*OP))//*param < 'A' || (*param > 'Z' && *param < 'a') || *param > 'z')
        return 0;
    for (i = 0; i < length; i++) {
        if (!isdigit(OP[i]) && !isalpha(OP[i]) && !isspace(OP[i])) {
            printf("invalid label name \n");
            return 0;
        }
    }
    if (length > 1) {
        if (length > 31) {
            printf("Label is too long\n");
            return 0;
        }
        if (isGuide(OP) || validate_command(OP) != -1 ||
            check_register_meth3(OP)) { /*need - or command or register or already is a label*/
            printf("invalid label name \n");
            return 0;
        }
    }
    return 1;
}


int check_int_meth0(char *OP, long *num) {
    if (isdigit(OP[1]) || OP[1] == 45 || OP[1] == 43) {
        int i = 0;
        char *ptr;
        *num = strtol(OP + 1, &ptr, 10);
        if (!check_non_white_char(ptr)) {
            printf("invalid integer, found non- numbers %s characters\n", ptr);
            return 0;

        }
        int min = (pow(2, 21) / 2) * -1;
        int max = (pow(2, 21) / 2) - 1;
        if (*num > max) {
            printf("the number you entered is too big %ld\n", *num);
            return 0;
        }
        if (*num < min) {
            printf("the number you entered is too small %ld\n", *num);
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}


// Convert an integer to binary (in a string)
void int2bin(int integer, char *binary, int n) {
    int i;
    for (i = 0; i < n; i++)
        binary[i] = (integer & (int) 1 << (n - i - 1)) ? '1' : '0';
    binary[n] = '\0';
    printf("this is the binary code inside int2bin %s\n", binary);
}

void fillin_BinOP(int funN, int opcodeN) {
    char function[6];
    char opcode[7];
    memset(curCNode->code.binCode, '0', 24);
    printf("the binary code of the first word before adding anything %s \n", curCNode->code.binCode);
    int2bin(funN, function, 5);
    int2bin(opcodeN, opcode, 6);
    printf("this is the binary code of func %s \n", function);
    printf("this is the binary code of opcode %s \n", opcode);
    strncpy(curCNode->code.binCode + 16, function, 5);
    strncpy(curCNode->code.binCode + 0, opcode, 6);
    printf("the binary code of the first word after opcode command is %s \n", curCNode->code.binCode);
    curCNode->code.adress = IC;
    strncpy(curCNode->code.binCode + 21, "1", 1);
    printf("the binary code of the first word command is %s \n", curCNode->code.binCode);

    if (labelFlag)
        strcpy(curCNode->code.opLabel, label);
    printf("the label of the command is %s\n", curCNode->code.opLabel);

}


void fillin_firstBinCode(int methCode, int regNum, int methInd, int regInd) {
    char methC[3], regN[4];
    int2bin(regNum, regN, 3);
    strncpy(curCNode->code.binCode + regInd, regN, 3);
    int2bin(methCode, methC, 2);
    printf("this is the output of int2bin for the method code %s\n", methC);
    printf("this is the output of int2bin for the register code num %s\n", regN);
    strncpy(curCNode->code.binCode + methInd, methC, 2);
    printf("the binary code of the first word after adding the registry and method command is %s \n",
           curCNode->code.binCode);
}

void fillin_extraBinCode(int num) {
    char binary[22];
    int2bin(num, binary, 21);
    IC++;
    curCNode->next= (CNode *) malloc(sizeof(CNode)); // set new node for int adress
    curCNode = curCNode->next;
    curCNode->code.adress = IC;
    memset(curCNode->code.binCode, '0', 24);
    curCNode->code.binCode[24] = '\0';
    strncpy(curCNode->code.binCode + 0, binary, 21); // complete the rest of the binary code
    printf("the binary code of extra word command is %s \n", curCNode->code.binCode);
}

void get_operand(int opInd, char *OP, int *meth, int methInd, int regInd) {
    int i;
    if (strlen(OP) >= 2) {
        if (OP[0] == 35 && meth[0] == 0)// direct method (number 0) for int operands
        {
            long num;
            if (check_int_meth0(OP, &num)) {
                printf("the operand %s is valid for %d method of command %s \n", OP, meth[0], opTable[opInd].opName);
                fillin_firstBinCode(0, 0, methInd, regInd);
                fillin_extraBinCode(num);
                strcpy(curCNode->code.binCode + 2, "1"); //set A=1 in in the ARE
            } else //operand started with # and no valid int was written after it
                return;
        } else if (check_register_meth3(OP) && meth[3] == 3) // method for register operands (number 3)
        {
            printf("the operand %s is valid for %d method of command %s \n", OP, meth[3], opTable[opInd].opName);
            fillin_firstBinCode(3, atoi(OP + 1), methInd, regInd);
        } else if (OP[0] == 38 && meth[2] == 2) //relative method (number 2)
        {
            if (check_label(OP + 1)) {
                printf("the operand %s is valid for %d method of command %s \n", OP, meth[2], opTable[opInd].opName);
                fillin_firstBinCode(2, 0, methInd, regInd);
                fillin_extraBinCode(0);
                strcpy(curCNode->code.operandLabel, OP + 1);
                strcpy(curCNode->code.binCode + 2, "1"); //set A=1 in in the ARE
            } else
                return; //operand started with & and no valid label was written after it
        } else {
            if (check_label(OP) && meth[1] == 1) {
                printf("the operand %s is valid for %d method of command %s \n", OP, meth[1], opTable[opInd].opName);
                fillin_firstBinCode(1, 0, methInd, regInd);
                fillin_extraBinCode(0);
                strcpy(curCNode->code.operandLabel, OP + 1);
            } else
                printf("The input command %s can't be applied with the selected sorce operand %s\n",
                       opTable[opInd].opName, OP);
        }
    } else {
        if (check_label(OP) && meth[1] == 1) {
            printf("the operand %s is valid for %d method of command %s \n", OP, meth[1], opTable[opInd].opName);
            fillin_firstBinCode(1, 0, methInd, regInd);
            fillin_extraBinCode(0);
            strcpy(curCNode->code.operandLabel, OP + 1);
        } else
            printf("The input command %s can't be applied with the selected sorce operand %s\n", opTable[opInd].opName,
                   OP);
    }
}


int check_non_valOP() {
    skipWhite();
    if (*p != '\0') {
        printf("A non valid number of operands was found extra operand %s was found\n", *p);
        return 0;
    } else
        return 1;

}


void get_opcode(char *opcode) {
    char sourceOP[MAX_LENGTH] = {'\0'}, targOP[MAX_LENGTH] = {'\0'};
    int opInd;
    char Code[MAX_LENGTH];
    strcpy(Code, opcode);

    if ((opInd = validate_command(Code)) != -1) {
        printf("the index of the command %s is %d\n", Code, opInd);
        int wrongComma = startWithComma();
        if (!wrongComma)
            return;
        if (opTable[opInd].oprndN == 2) {
            getVar();
            if (!checkComma())
                return;
            strcpy(sourceOP, param);
            printf("this is the source operand %s of the command %s\n", sourceOP, Code);

            getParam();
            if (!check_non_valOP())
                return;
            strcpy(targOP, param);
            printf("this is the targ operand %s of the command %s\n", targOP, Code);


            if (sourceOP[0] != '\0' && targOP[0] != '\0') {//&& check_non_valOP) {
                fillin_BinOP(opTable[opInd].func, opTable[opInd].opcode);
                get_operand(opInd, sourceOP, opTable[opInd].sourceMeth, 6, 8);
                get_operand(opInd, targOP, opTable[opInd].targMeth, 11, 13);
                printf("----------------------------------end of the command %s\n", Code);
            }
        } else if (opTable[opInd].oprndN == 1) {
            getParam();
            if (!check_non_valOP())
                return;
            strcpy(targOP, param);
            printf("this is the targ operand %s of the command %s\n", targOP, Code);

            //getParam();
            //strcpy(nonValOP, param);
            if (targOP[0] != '\0') { //&& check_non_valOP) {
                fillin_BinOP(opTable[opInd].func, opTable[opInd].opcode);
                get_operand(opInd, targOP, opTable[opInd].targMeth, 11, 13);
                printf("----------------------------------end of the command %s\n", Code);

            }
        } else if (opTable[opInd].oprndN == 0) {
            //getParam();
            //strcpy(nonValOP, param);
            //if (check_non_valOP)
            if (!check_non_valOP())
                return;
            printf("no targ operand of the command %s\n", Code);
            fillin_BinOP(opTable[opInd].func, opTable[opInd].opcode);
            printf("----------------------------------end of the command %s\n", Code);
        }
    } else {
        printf("The command %s entered is not valid, Please eneter a valid command \n", Code);
        printf("----------------------------------end of the command %s\n", Code);
        return;
    }
    curCNode->next = (CNode *) malloc(sizeof(CNode)); // set new node for int adress
    curCNode = curCNode->next;
    curCNode->code.binCode[24] = '\0';
    IC++;

}
