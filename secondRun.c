#include "secondRun.h"

void secondRun() {
    fseek(fp, 0, SEEK_SET);
    while (fgets(line, MAX_LENGTH, fp)) {
        p = line;
        getParam();
        if(inLabelTab(param))
            getParam();
        switch (isGuide(param)){
            case 4:{


            }break;
        }
    }
    free(curSNode);
    free(curDNode);
    free(curCNode);
    free(sHead);
    free(dHead);
    free(Chead);
}
