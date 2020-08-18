#include "functionsForAssembler.h"
#include "main.h"

int main(int argc, char* argv[] ) {
    int i;
    if (argc<2)/*if there is less filenames then expected */
        fprintf(stdout, "files are missing\n");
    else {
        strcpy(filename, argv[0]);/*copy the name of the file - need to check it */
        for (i = 1; i < argc; i++) {
            strcpy(input,argv[i]);/*copy and add .as to the input*/
            strcat(input,".as");
            if(!(fp = fopen(input,"r")))/*file dose not exist*/
                fprintf(stdout, "File not found\n");
            else{
                firstRun();
                secondRun();
                fclose(fp);
            }

        }
    }
    exit(0);
}
