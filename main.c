
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(int argc, char* argv[]){
    FILE* ptr;
    STATE_MACHINE_RETURN_VALUE state;
    char ch;


    // Opening file in reading mode
    ptr = fopen(argv[1], "rb");
    if (NULL == ptr) {
        printf("file can't be opened \n");
        exit(1);
    }

    ch = fgetc(ptr);
    do{
        do {
            state = at_command_parser(ch);
            if(state == STATE_MACHINE_READY_WITH_ERROR){
                printf("\n%s\n",getStringFromStateMachine(state));
                fclose(ptr);
                exit(1);
            }
            if(state != STATE_MACHINE_READY_OK)
                ch = fgetc(ptr);
            printf("%c", ch);
        } while (state != STATE_MACHINE_READY_OK);
        printf("\n%s\n",getStringFromStateMachine(state));

    } while (ch != EOF);

    fclose(ptr);

}