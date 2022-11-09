
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(int argc, char* argv[]){
    FILE* ptr;
    STATE_MACHINE_RETURN_VALUE state = STATE_MACHINE_NOT_READY;
    char ch;


    // Opening file in reading mode
    ptr = fopen(argv[1], "rb");
    if (NULL == ptr) {
        printf("file can't be opened \n");
        exit(1);
    }

    ch = fgetc(ptr);
    do{
        if(ch != 0x0D) ch = fgetc(ptr);
        else do {
            state = at_command_parser(ch);
            if(state == STATE_MACHINE_READY_WITH_ERROR){
                printf("\033[0;31m");
                printf("\nReturn with error");
                printf("\033[0;0m");
                fclose(ptr);
                exit(1);
            }
            if(state != STATE_MACHINE_READY_OK)
                ch = fgetc(ptr);
            // printf("%c", ch);
        } while (state != STATE_MACHINE_READY_OK);

        if(state != STATE_MACHINE_NOT_READY) printf("\n%s\n",getStringFromStateMachine(state));
    } while (ch != EOF);

    if(state == STATE_MACHINE_NOT_READY) printf("\n%s :never get <CR> character\n",getStringFromStateMachine(state));
    fclose(ptr);

}