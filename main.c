#include "functionsForAssembler.h"
#include "main.h"



int main(int argc, char* argv[] ) {
    runAssembler(argc, argv);
    exit(0);
}

void runAssembler(int argc, char* argv[]) {
    char input[MAX_LENGTH];
    int i;
    if (argc<2)/*if there is less filenames then expected */
        fprintf(stdout, "files are missing\n");
    else {
        strcpy(filename, argv[0]);/*copy the name of the file - need to check it */
        for (i = 1; i < argc; i++) {
            strcpy(inputOrg,argv[i]);/*copy and add .as to the input*/
            strcpy(input,inputOrg);
            strcat(input,".as");
            if(!(fp = fopen(input,"r")))/*file dose not exist*/
                fprintf(stdout, "File not found\n");
            else{
                fprintf(stdout, "\n\t-----Reading file: %s------\n", input);
                firstRun();
                secondRun();
                fclose(fp);
            }

        }
    }
}
