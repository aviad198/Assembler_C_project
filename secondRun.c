#include "secondRun.h"
int fileLine=0;
void secondRun() {
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
                    printf("Error in entry, no such label\n");
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
        addLabelOperand();
    }
    if (goodFile) {


        if (hasExtern) {

        }

        if (hasEntry) {
            const char entryW[10] = "entry";
            curSNode = sHead;
            while (curSNode != NULL) {
                if (strstr(curSNode->sign.car, entryW)) {
                    printf("%s\t%07d\n", curSNode->sign.label, curSNode->sign.value);
                }
                curSNode = curSNode->next;
            }
        }

    }
    free(curSNode);
    free(curDNode);
    free(curCNode);
    free(sHead);
    free(dHead);
    free(Chead);

}
    void addLabelOperand() {
        char adress[22];
        symboleTabel *tempSNode;
        tempSNode = sHead;
        CNode *tempCNode;
        tempCNode = Chead;
        while (tempCNode != NULL) {
            while (tempSNode != NULL) {
                //printf("the sign labl of the table of symbols %s\n", tempNode-> sign.label);
                if (tempCNode->code.operandLabel && strcmp(tempCNode->code.operandLabel, tempSNode->sign.label) == 0) {
                    //tempSNode->sign.value += IC; // make sure to move the IC to thr right header
                    int2bin(tempSNode->sign.value, adress, 21);/// assuming that IC was add to DC
                    strncpy(tempCNode->code.binCode + 0, 21, adress);
                    if (strstr(tempSNode->sign.car, ".entry") == 0) //its entry or somthing else
                        strncpy(tempCNode->code.binCode + 23, 1, "1");
                    else
                        strncpy(tempCNode->code.binCode + 22, 1, "1");
                } else {
                    fprintf(stdout, "the operand label doesnt mutch the lables in the symbol table labels\n");
                    return;
                }
                tempSNode = tempSNode->next;
            }

            tempCNode = tempCNode->next;
        }
    }
