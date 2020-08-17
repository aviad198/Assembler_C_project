#include "secondRun.h"

void secondRun() {
    fseek(fp, 0, SEEK_SET);
    while (fgets(line, MAX_LENGTH, fp)) {
        p = line;
        getParam();
        if (inLabelTab(param))
            getParam();
        switch (isGuide(param)) {
            case 4: {
                if (isLabel(param)) {
                    curSNode = sHead;
                    while (curSNode != NULL) {
                        if (strcmp(curSNode->sign.label, label) == 0) {
                            strcat(curSNode->sign.car, ", entry");
                            continue;
                        }
                    }
                    printf("Error in entry, no such label");
                    continue;
                } else
                    printf("not a valid label");
            }
                break;

        }
    }
    if(goodFile) {
        if (hasExtern) {

        }

        if (hasEntry){

        }

    }
    free(curSNode);
    free(curDNode);
    free(curCNode);
    free(sHead);
    free(dHead);
    free(Chead);
}
