
#include "functionsForAssembler.h"

/*checks of its a lable to turn on label flag*/
bool isLabel(char param[50]) {
    int length = strlen(param);
    if (length > 1 && param[length - 1] == ':') {
        param[length-1]='\0';
        strcpy(label, param);
        return 1;
    }
    return 0;
}


/*gets parameter from line*/
void getParam() {
    skipWhite();
    sscanf(p, "%s", param);
    /* if ((sscanf(p, "%s", param))== 0)
     {
       *param = '\0';
     }*/
    skipWhite();
    pointAfterParam();
}


/*move pointer after last parameter*/
void pointAfterParam() {
    //if (*p != '\n' && *p != '\0' )
    p += strlen(param);
}

/*points pointer p after white space*/
void skipWhite() {
    while (isspace(*p))
        p++;
}

/*if its a guide command*/
int isGuide(char param[50]) {
    if (!strcmp(param, ".data"))
        return 1;
    else if (!strcmp(param, ".string"))
        return 2;
    else if (!strcmp(param, ".entry"))
        return 3;
    else if (!strcmp(param, ".extern"))
        return 4;

    return 0;
}




bool inLabelTab (char param[50]){
    symboleTabel *tempNode;
    tempNode = sHead;
    while (tempNode != NULL){
        //printf("the sign labl of the table of symbols %s\n", tempNode-> sign.label);
        if (strcmp(tempNode->sign.label, param)==0) {
            return 1;
        }
        tempNode = tempNode->next;
    }
    return 0;
}

/*adds to sign table*/
void addSign(char label[50], char character[50], int value) {

    if (inLabelTab(label)) {
        printf("This label is alradey decalared");
        return;
    }

    strcpy(curSNode->sign.label, label);
    curSNode->sign.value = (value);
    strcpy(curSNode->sign.car, character);

    symboleTabel *newSNode = (symboleTabel *) malloc(sizeof(symboleTabel));
    curSNode->next = newSNode;
    curSNode = newSNode;
    curSNode->sign.label[0] = '\0';

}

bool isNum (char * param){
    int num;
    char *end;

    strtol(param, &end,10);
    if (end<= (param+strlen(param))){
        return 0;
    }
    return 1;
}