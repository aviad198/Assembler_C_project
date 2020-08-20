#include "secondRun.h"
int fileLine;



void secondRun() {
    fileLine=0;
    fseek(fp, 0, SEEK_SET);
    while (fgets(line, MAX_LENGTH, fp)) {
        ++fileLine;
        p = line;
        getParam();
        if (inLabelTab(param))
            getParam();
        switch (isGuide(param)) {
            /*entry command*/
            case 3: {
                getParam();
                if (!inLabelTab(param)) {
                    fprintf(stdout,"Line %d: Error in entry, no such label\n", fileLine);
                    break;
                }
                curSNode = sHead;
                while (curSNode != NULL) {
                    if (strcmp(curSNode->sign.label, param) == 0) {
                        strcat(curSNode->sign.car, ", entry");
                        hasEntry = true;
                    }
                    curSNode = curSNode->next;
                }

            }
                break;

        }


    }
//    curSNode = sHead;
//    while (curSNode != NULL) {
//        printf("%s\t%d\t%s\n",curSNode->sign.label,curSNode->sign.value,curSNode->sign.car);
//        curSNode = curSNode->next;
//    }

    addLabelOperand();

    if (goodFile) {
        char objF[100];
        char entF[100];
        char extF[100];
        strcpy(objF,inputOrg);
        strcpy(entF,inputOrg);
        strcpy(extF,inputOrg);
        if(!(objFile = fopen(strcat(objF,".ob"),"w"))){/*file dose not exist*/
            fprintf(stdout, "File cant be created\n");
            return;
        }
        printObjFile();
    fclose(objFile);
       if (hasExtern){
        if(!(extFile = fopen(strcat(extF,".ext"),"w"))){/*file dose not exist*/
            fprintf(stdout, "File cant be created\n");
            return;}
           symboleTabel *tempSNode;
           CNode *tempCNode;
           tempCNode = Chead;
           while (tempCNode != NULL) {
               tempSNode = inLabelTab(tempCNode->code.operandLabel);
               if(tempSNode!=NULL){
                   if (strstr(tempSNode->sign.car, "external") != NULL){
                       fprintf(extFile,"%s\t%07d\n",tempCNode->code.operandLabel,tempCNode->code.adress);
                   }
               }
               tempCNode =tempCNode->next;
           }
           fclose(extFile);
           }


        if (hasEntry) {
            if(!(entFile = fopen(strcat(entF,".ent"),"w"))){/*file dose not exist*/
                fprintf(stdout, "File cant be created\n");
                return;
            }
            const char entryW[10] = "entry";
            curSNode = sHead;
            while (curSNode->next != NULL) {
                if (strstr(curSNode->sign.car, entryW)) {
                    fprintf(entFile,"%s\t%07d\n", curSNode->sign.label, curSNode->sign.value);
                }
                curSNode = curSNode->next;
            }
            fclose(entFile);
       }

    }
    free(curSNode);
    free(curDNode);
    free(curCNode);
    free(sHead);
    free(dHead);
    free(Chead);

}
void addLabelOperand(){
    char adress[22];
    symboleTabel *tempSNode;
    CNode *tempCNode;
    tempCNode = Chead;
    while (tempCNode != NULL) {
            //printf("the sign label of the table of symbols %s\n", tempNode-> sign.label);
            if (tempCNode->code.operandLabel[0]!='\0' && tempCNode->code.operandLabel[0] != '&') {
                tempSNode = inLabelTab(tempCNode->code.operandLabel);
                if(tempSNode==NULL) {
                    fprintf(stdout, "the operand label %s doesnt match the labels in the symbol table labels\n",tempCNode->code.operandLabel);
                    goodFile =false;
                    tempCNode = tempCNode->next;
                    continue;
                }

                int2bin(tempSNode->sign.value, adress, 21);/// assuming that IC was add to DC
                strncpy(tempCNode->code.binCode + 0, adress, 21);
                if (strstr(tempSNode->sign.car, "external") != NULL) //its entry or somthing else
                    strncpy(tempCNode->code.binCode + 23, "1", 1);
                else
                    strncpy(tempCNode->code.binCode + 22, "1", 1);
            }
            else if(tempCNode->code.operandLabel[0] == '&') {
                int distance;
               tempSNode = inLabelTab(tempCNode->code.operandLabel+1);
                if(tempSNode==NULL) {
                    fprintf(stdout, "the operand label %s doesnt match the labels in the symbol table labels\n",tempCNode->code.operandLabel);
                    goodFile =false;
                    tempCNode = tempCNode->next;
                    continue;
                }
                distance = tempSNode->sign.value - tempCNode->code.adress+1;
                int2bin(distance, adress, 21);
                strncpy(tempCNode->code.binCode + 0,adress , 21);

            } else{
                tempCNode = tempCNode->next;
                continue;
            }

        tempCNode = tempCNode->next;
        }
    }



void printObjFile(){
    CNode *tempCNode;
    dataNode *tempDNode;
    tempCNode = Chead;
    tempDNode = dHead;
    while (tempCNode->next != NULL) {
        unsigned long value = strtoul(tempCNode->code.binCode, NULL, 2);
        // convert integer to hex string
        //fprintf(objFile,"%s\n",tempCNode->code.binCode);
        fprintf(objFile,"%07d\t%06x\n", tempCNode->code.adress,value);
        tempCNode = tempCNode->next;
    }
    while (tempDNode->next != NULL){
        fprintf(objFile,"%07d\t%06x\n", IC ,tempDNode->data);
        IC++;
        tempDNode = tempDNode->next;
    }

}
