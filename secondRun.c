#include "secondRun.h"

void secondRun() {
    fseek(fp, 0, SEEK_SET);
    while (fgets(line, MAX_LENGTH, fp)) {
        p = line;
        getParam();
        if (inLabelTab(param))
            getParam();
        switch (isGuide(param)) {
            /*entry command*/
            case 3: {
                getParam();
                if(!inLabelTab(param)){
                    printf("Error in entry, no such label\n");
                    break;
                }
                    curSNode = sHead;
                    while (curSNode != NULL) {
                        if (strcmp(curSNode->sign.label, param) == 0) {
                            strcat(curSNode->sign.car, ", entry");
                            hasEntry = true;
                        }
                        curSNode =curSNode->next;
                    }

            }break;

        }
    }
 //   if(goodFile) {


        if (hasExtern) {

        }

        if (hasEntry){
        const char entryW[10] = "entry";
        curSNode = sHead;
            while (curSNode != NULL) {
                if (strstr(curSNode->sign.car, entryW)){
                    printf("%s\t%07d\n",curSNode->sign.label,curSNode->sign.value+100);
                }
                curSNode =curSNode->next;
            }
        }

 //   }
    free(curSNode);
    free(curDNode);
    free(curCNode);
    free(sHead);
    free(dHead);
    free(Chead);
}
