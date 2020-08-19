
#include "FirstRun.h"

//IC = 100;
//DC = 0;
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
int lineCounter = 0;

/*start first run */
void firstRun() {
    goodFile = true;
    hasExtern = false;
    hasEntry = false;
    /*new heads for linked lists */
    if (!(sHead = (symboleTabel *) calloc(1, sizeof(symboleTabel)))) {
        fprintf(stdout, "line %d: \"Memory allocation failed\"\n", lineCounter);
        return;
    }
    curSNode = sHead;
    //curSNode->sign.label[0] = '\0';
    if (!(dHead = (dataNode *) calloc(1, sizeof(dataNode)))) {
        fprintf(stdout, "line %d: \"Memory allocation failed\"\n", lineCounter);
        return;
    }
    curDNode = dHead;
    if (!(Chead = (CNode *) calloc(1, sizeof(CNode)))) {
        fprintf(stdout, "line %d: \"Memory allocation failed\"\n", lineCounter);
        return;
    }
    curCNode = Chead;
    fillinOpTable();
    /*while file has a line*/
    while (fgets(line, MAX_LENGTH, fp)) {
        p = NULL;
        param[0] = '\0';
        lineCounter++;
        /*points to first letter in line*/
        p = line;
        skipWhite();
        if (*p == ';') {
            continue;
        }
        getParam();
        /*turn on flag if there is a sign*/
        if (labelFlag = isLabel(param) == 1) {
            if (inLabelTab(label)) {
                fprintf(stdout, "line %d: already is label with same name\n", lineCounter);
                goodFile = false;
                continue;
            }

        }
        /*gets next param*/
        if (labelFlag) {
            getParam();
        }
        /*if its a guide command (.data/.string/.extern/.entry)*/
        switch (isGuide(param)) {
            case 5:
                continue;
                /*.data guide */
            case 1: {
                if (labelFlag)
                    addSign(label, "data", DC);
                addData();
            }
                break;
                /*.string guide */
            case 2: {

                if (labelFlag)
                    addSign(label, "data", DC);

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
                    fprintf(stdout, "line %d: Warning Label before entry command\n", lineCounter);
                }
                getParam();
                if (*param == '\n') {
                    fprintf(stdout, "line %d: missing extern label", lineCounter);
                    continue;
                }
                if (!inLabelTab(param)) {
                    addSign(param, "external", 0);
                }
                hasExtern = true;
            }
                break;

            default: {
                if (labelFlag) {
                    if (inLabelTab(label))
                        continue;
                    addSign(label, "code", IC);

                }

                getCommand(param);
            }
                break;
        }
/*go over symbolTabel and add 100 to value*/
    }
    curSNode = sHead;
    while (curSNode != NULL) {
        if (strstr(curSNode->sign.car, "data") || strstr(curSNode->sign.car, "string")) {
            curSNode->sign.value += IC;
        }
        curSNode = curSNode->next;
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
        fprintf(stdout, "line %d: missing data\n", lineCounter);
        goodFile = false;
        return;
    }
    /*dosnt start string with " */
    if (*p != '\"') {
        fprintf(stdout, "line %d: has to start with \" \n", lineCounter);
        goodFile = false;
        return;
    }
    p++;
    skipWhite();
    if (!isascii(*p)) {
        fprintf(stdout, "line %d: missing char\n", lineCounter);
        goodFile = false;
        return;
    }
    /*while not end of line or end of string*/
    while (*p != '\0' && *p != '\"') {

        /*adds the char to data linked list*/
        curDNode->data.ch = *p;
        if (!(curDNode->next = (dataNode *) calloc(1, sizeof(dataNode)))) {
            fprintf(stdout, "line %d: Memory allocate failure\n", lineCounter);
            goodFile = false;
            return;
        }


        curDNode = curDNode->next;

        DC++;
        p++;
        skipWhite();
        check++;
    }
    /*check if last char is "*/
    if (*p != '\"') {
        fprintf(stdout, "line %d: has to end with \" \n", lineCounter);
        goodFile = false;
        return;
    }
    /*adds the char \0 at the end of linked list*/
    curDNode->data.ch = '\0';
    if (!(curDNode->next = (dataNode *) calloc(1, sizeof(dataNode)))) {
        fprintf(stdout, "line %d: Memory allocate failure\n", lineCounter);
        goodFile = false;
        return;
    }

    curDNode = curDNode->next;
    DC++;

    p++;
    skipWhite();
    if (*p != '\0') {
        fprintf(stdout, "line %d: extra input after .string\n", lineCounter);
        goodFile = false;
        return;
    }

}


int startWithComma() {
    skipWhite();
    /*if starts with comma*/
    if (*p == ',') {
        fprintf(stdout, "line %d: statement cannot start with a comma\n", lineCounter);
        goodFile = false;
        return 1;
    }
    /*if empty data*/
    if (*p == '\0') { // ita was if (*p == '\n') and i changed it because it didnt worked
        fprintf(stdout, "line %d: missing data\n", lineCounter);
        goodFile = false;
        return 1;
    }
    return 0;

}


/*adds data to data linked list*/

void addData() {
    if (startWithComma())
        return;
    getVar();
    if (!insertData())
        return;

/*while not end of line*/
    while (*p != '\0') {
        /*if missing comma or mult commas*/
        if (!checkComma()) {
            return;

        } else {
            getVar();
            if (!insertData())
                return;
        }

    }

}


bool insertData() {
    if (!isNum(param)) {
        fprintf(stdout, "line %d: wrong input for data\n", lineCounter);
        goodFile = false;
        return 0;
    } else {
        /*insert data*/
        curDNode->data.num = atoi(param);

        if (!(curDNode->next = (dataNode *) calloc(1, sizeof(dataNode)))) {
            fprintf(stdout, "line %d: Memory allocate failure\n", lineCounter);
            goodFile = false;
            return 0;
        }
        curDNode = curDNode->next;
        DC++;
        return 1;
    }
}

/*get a var diveded by commas */
void getVar() {
    skipWhite();
    sscanf(p, "%[^,]", param);
    sscanf(param, "%s", param);
    pointAfterParam();
    skipWhite();
}

/*checks if has comma ot not or have multiple commas*/
bool checkComma() {
    int counter = 0;
    skipWhite();
    /*check if the first letter (after skipWhite is a comma and looks for multiple commas*/
    while (*p == ',') {
        counter++;
        p++;
        skipWhite();
    }
    /*missing comma*/
    if (counter == 0) {
        fprintf(stdout, "line %d: Missing comma\n", lineCounter);
        goodFile = false;
        return false;
        /*more then one comma*/
    } else if (counter > 1) {
        fprintf(stdout, "line %d: Multiple consecutive commas\n", lineCounter);
        goodFile = false;
        return false;
    }
    /*if no data after comma*/
    if (*p == '\0') {
        fprintf(stdout, "line %d: Missing Data after comma\n", lineCounter);
        goodFile = false;
        return false;
    }
    return true;
}



//bool validat_command(

void fillinOpTable() {
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


int validateCommand(char *opcode) {
    int i;
    for (i = 0; i < TOTAL_OP; i++) {
        if (!(strcmp(opcode, opTable[i].opName))) {
            //printf("the command %s is valid\n", opcode);
            return i;
        }
    }
    return -1;
}

bool checkNonWhiteChar(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (!isspace(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}

bool checkRegisterMeth3(char *OP) {
    if (OP[0] == 114 && isdigit(OP[1])) {
        if (atoi(OP + 1) < 0 || atoi(OP + 1) > 7)
            return 0;
        if (strlen(OP) > 2 && !checkNonWhiteChar(OP + 2))
            return 0;
    } else
        return 0;
    return 1;
}


bool checkLabel(char *label) {
    int i;
    int length = strlen(label);

    if (!isalpha(*label)) {//*param < 'A' || (*param > 'Z' && *param < 'a') || *param > 'z')
        fprintf(stdout, "line %d: label has to start with letter\n", lineCounter);
        goodFile = false;
        return 0;
    }
    if (length > 31) {
        fprintf(stdout, "line %d: label is too long\n", lineCounter);
        goodFile = false;
        return 0;
    }
    if (isGuide(label)) { /*need - or command or register or already is a label*/
        fprintf(stdout, "line %d:unvalid name for label\n", lineCounter);
        goodFile = false;
        return 0;
    }
    for (i = 0; i < length; i++) {
        if (!isalnum(*(label + i)) && !isspace(*(label + i))) {
            fprintf(stdout, "line %d:label has to be contain alpha/num\n", lineCounter);
            goodFile = false;
            return 0;
        }
    }
    if (isGuide(label) || validateCommand(label) != -1 ||
        checkRegisterMeth3(label)) { /*need - or command or register or already is a label*/
        fprintf(stdout, "line %d:invalid label name\n", lineCounter);
        return 0;
    }
    return 1;
}


bool checkIntMeth0(char *OP, long *num) {
    if (isdigit(OP[1]) || OP[1] == 45 || OP[1] == 43) {
        int i = 0;
        char *ptr;
        *num = strtol(OP + 1, &ptr, 10);
        if (!checkNonWhiteChar(ptr)) {
            fprintf(stdout, "line %d: invalid integer, found non- numbers %s characters\n", lineCounter, ptr);
            goodFile = false;
            return 0;

        }
        int min = (pow(2, 21) / 2) * -1;
        int max = (pow(2, 21) / 2) - 1;
        if (*num > max) {
            fprintf(stdout, "line %d: the number you entered is too big %ld\n", lineCounter, *num);
            goodFile = false;
            return 0;
        }
        if (*num < min) {
            fprintf(stdout, "line %d: the number you entered is too small %ld\n", lineCounter, *num);
            goodFile = false;
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

void fillinBinOp(int funN, int opcodeN) {
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
    printf("the adress of the first bincode is %d\n", curCNode->code.adress);
    printf("the label of the command of the first bincode is %s\n", curCNode->code.opLabel);

}


void fillinFirstBinCode(int methCode, int regNum, int methInd, int regInd, CNode *firstCNode) {
    CNode *firstBin;
    if (firstCNode == NULL)
        firstBin = curCNode;
    else
        firstBin = firstCNode;

    char methC[3], regN[4];
    int2bin(regNum, regN, 3);
    strncpy(firstBin->code.binCode + regInd, regN, 3);
    int2bin(methCode, methC, 2);
    printf("this is the output of int2bin for the method code %s\n", methC);
    printf("this is the output of int2bin for the register code num %s\n", regN);
    strncpy(firstBin->code.binCode + methInd, methC, 2);
    printf("the binary code of the first word after adding the registry and method command is %s \n",
           firstBin->code.binCode);
    printf("the adress of the first bincode is %d\n", firstBin->code.adress);
}

void fillinExtraBinCode(int num) {
    char binary[22];
    int2bin(num, binary, 21);
    IC++;
    if (!(curCNode->next = (CNode *) calloc(1, sizeof(CNode)))) {// set new node for int adress
        fprintf(stdout, "line %d: memory allocation failed %s\n", lineCounter);
        goodFile = false;
        return;
    }
    curCNode = curCNode->next;
    curCNode->code.adress = IC;
    memset(curCNode->code.binCode, '0', 24);
    //curCNode->code.binCode[24] = '\0';
    strncpy(curCNode->code.binCode + 0, binary, 21); // complete the rest of the binary code
    printf("the binary code of extra word command is %s \n", curCNode->code.binCode);
    if (labelFlag)
        strcpy(curCNode->code.opLabel, label);
    printf("the adress of the extra bincode is %d\n", curCNode->code.adress);
    printf("the label of the command of the extra bincode is %s\n", curCNode->code.opLabel);
}
//getOperand command check if which type of operand was recieved and check if it is valid
bool getOperand(int opInd, char *arg, int *meth, int methInd, int regInd, int *isExtraBin, CNode *firstCNode) {
    //the operand type is #number
    if (strlen(arg) >= 2) {
        if (arg[0] == 35 && meth[0] == 0)// direct method (number 0) for int operands
        {
            long num;
            if (checkIntMeth0(arg, &num)) {
                printf("the operand %s is valid for %d method of command %s \n", arg, meth[0], opTable[opInd].opName);
                fillinFirstBinCode(0, 0, methInd, regInd, firstCNode);
                *isExtraBin = 1;
                fillinExtraBinCode(num);
                strncpy(curCNode->code.binCode + 21, "1", 1); //set A=1 in in the ARE

                return 0;
            } else //operand started with # and no valid int was written after it
                return 1;
            //the operand type is register r(number: 0-7)
        } else if (checkRegisterMeth3(arg) && meth[3] == 3) // method for register operands (number 3)
        {
            printf("the operand %s is valid for %d method of command %s \n", arg, meth[3], opTable[opInd].opName);
            fillinFirstBinCode(3, atoi(arg + 1), methInd, regInd, firstCNode);
            return 0;

            /*the operand type is &label*/
        } else if (arg[0] == 38 && meth[2] == 2) //relative method (number 2)
        {
            if (checkLabel(arg + 1)) {
                printf("the operand %s is valid for %d method of command %s \n", arg, meth[2], opTable[opInd].opName);
                fillinFirstBinCode(2, 0, methInd, regInd, firstCNode);
                *isExtraBin = 1;
                fillinExtraBinCode(0);
                strcpy(curCNode->code.operandLabel, arg);
                strncpy(curCNode->code.binCode + 21, "1", 1); //set A=1 in in the ARE
                return 0;
            } else
                return 1; //operand started with & and no valid label was written after it
        } else {
            /*the operand type is a label*/
            if (checkLabel(arg) && meth[1] == 1) {
                printf("the operand %s is valid for %d method of command %s \n", arg, meth[1], opTable[opInd].opName);
                fillinFirstBinCode(1, 0, methInd, regInd, firstCNode);
                *isExtraBin = 1;
                fillinExtraBinCode(0);
                strcpy(curCNode->code.operandLabel, arg);
                return 0;
            } else
                fprintf(stdout, "line %d: The input command %s can't be applied with the selected source operand %s\n",
                        lineCounter, opTable[opInd].opName, arg);
            goodFile = false;
            return 1;
        }
    } else {
        //the operand type is a label
        if (checkLabel(arg) && meth[1] == 1) {
            printf("the operand %s is valid for %d method of command %s \n", arg, meth[1], opTable[opInd].opName);
            fillinFirstBinCode(1, 0, methInd, regInd, firstCNode);
            *isExtraBin = 1;
            fillinExtraBinCode(0);
            strcpy(curCNode->code.operandLabel, arg);
            return 0;
        } else
            fprintf(stdout, "line %d: The input command %s can't be applied with the selected source operand %s\n",
                    lineCounter, opTable[opInd].opName, arg);
        goodFile = false;
        return 1;

    }
}


bool checkNonValOp() {
    skipWhite();
    if (*p != '\0') {
        fprintf(stdout, "line %d: a non valid number of operand was found \n", lineCounter);
        goodFile = false;
        return 0;
    } else
        return 1;

}

//getCommand function recieves as input a command argument and if the command is valid the function check how many operands the specfic
//command is suppose to get and recieve a source and a target operands and call the function getOperand for each of them recpectivly
void getCommand(char *commandArg) {
    CNode *firstCNode;
    int isExtraBin = 0;
    char sourceOP[MAX_LENGTH] = {'\0'}, targOP[MAX_LENGTH] = {'\0'};
    int opInd;
    char command[MAX_LENGTH];
    strcpy(command, commandArg);
//check if the command is valid-exist in the command table
    if ((opInd = validateCommand(command)) != -1) {
        printf("the index of the command %s is %d\n", command, opInd);
        //command that receives exactly 2 operands
        if (opTable[opInd].oprndN == 2) {
            if (startWithComma())
                return;
            getVar();
            if (!checkComma())
                return;
            strcpy(sourceOP, param);
            printf("this is the source operand %s of the command %s\n", sourceOP, command);

            getParam();
            if (!checkNonValOp())
                return;
            strcpy(targOP, param);
            printf("this is the targ operand %s of the command %s\n", targOP, command);
            //if (sourceOP[0] != '\0' && targOP[0] != '\0') {
            fillinBinOp(opTable[opInd].func, opTable[opInd].opcode);
            firstCNode = curCNode;
            if (getOperand(opInd, sourceOP, opTable[opInd].sourceMeth, 6, 8, &isExtraBin, NULL))
                return;
            if (isExtraBin) {
                if (getOperand(opInd, targOP, opTable[opInd].targMeth, 11, 13, &isExtraBin, firstCNode))
                    return;
            } else {
                if (getOperand(opInd, targOP, opTable[opInd].targMeth, 11, 13, &isExtraBin, NULL))
                    return;
            }
            printf("----------------------------------end of the command %s\n", command);
            //}
            // command that receives exactly 1 operand
        } else if (opTable[opInd].oprndN == 1) {
            if (startWithComma())
                return;
            getParam();
            if (!checkNonValOp())
                return;
            strcpy(targOP, param);
            printf("this is the targ operand %s of the command %s\n", targOP, command);

            //getParam();
            //strcpy(nonValOP, param);
            //if (targOP[0] != '\0') {
            fillinBinOp(opTable[opInd].func, opTable[opInd].opcode);
            if (getOperand(opInd, targOP, opTable[opInd].targMeth, 11, 13, &isExtraBin, NULL))
                return;
            printf("----------------------------------end of the command %s\n", command);

            //} //command that receives no operands
        } else if (opTable[opInd].oprndN == 0) {
            //getParam();
            //strcpy(nonValOP, param);
            //if (checkNonValOp)
            if (!checkNonValOp())
                return;
            printf("no targ operand of the command %s\n", command);
            fillinBinOp(opTable[opInd].func, opTable[opInd].opcode);
            printf("----------------------------------end of the command %s\n", command);
        }
    } else {
        fprintf(stdout, "line %d: The command %s entered is not valid, Please eneter a valid command\n", lineCounter,
                command);
        goodFile = false;
        printf("----------------------------------end of the command %s\n", command);
        return;
    }
    if (!(curCNode->next = (CNode *) calloc(1, sizeof(CNode)))) {// set new node for int adress
        fprintf(stdout, "line %d: Memory allocate failed\n", lineCounter);
        goodFile = false;
        return;
    }

    curCNode = curCNode->next;
    IC++;
}




