
#include "parser.h"
#include <stdio.h>
#include <string.h>

AT_COMMAND_DATA data;

int belongToInterval(uint8_t ch){
    return (ch >= 32) && (ch <= 125);
}

const char* getStringFromStateMachine(STATE_MACHINE_RETURN_VALUE ret){
    static char buffer[35];
    if (ret == STATE_MACHINE_READY_OK)
        sprintf(buffer, "STATE_MACHINE_READY_OK");
    else if (ret == STATE_MACHINE_READY_WITH_ERROR)
        sprintf(buffer, "STATE_MACHINE_READY_WITH_ERROR");
    else if (ret == STATE_MACHINE_NOT_READY)
        sprintf(buffer, "STATE_MACHINE_NOT_READY");
    return buffer;
}

void resetMemory(void){
    data.line_count = 0;
    for (int i = 0; i < AT_COMMAND_MAX_LINES; i++)
        memset(data.data[i], 0, sizeof(data.data[i]));
}

void addCharacter(uint8_t character, uint32_t* lineIndex, uint32_t* stringIndex){
    if (*lineIndex < AT_COMMAND_MAX_LINES) {
        if (*stringIndex < AT_COMMAND_MAX_LINE_SIZE - 1) {
            data.data[*lineIndex][(*stringIndex)++] = character;
        }
        else if (*stringIndex == AT_COMMAND_MAX_LINE_SIZE - 1){
            data.data[*lineIndex][*stringIndex] = '\0';
            (*stringIndex)++;
        }
    }
}

STATE_MACHINE_RETURN_VALUE at_command_parser(uint8_t current_character){
    static uint32_t state = 0;
    static uint32_t lineIndex = 0;
    static uint32_t stringIndex = 0;

    switch (state){
        case 0: if (current_character == 0x0D){state = 1; resetMemory();} break;
        case 1:
            if (current_character == 0x0A) state = 2;
            else{
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        case 2:{
            if (current_character == '+') state = 3;
            else if (current_character == 'O') state = 12;
            else if (current_character == 'E') state = 7;
            else{
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 3:{

            if (belongToInterval(current_character)){
                state = 4;
                addCharacter(current_character, &lineIndex, &stringIndex);
            }
            else{
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 4:{

            if (belongToInterval(current_character)){
                addCharacter(current_character, &lineIndex, &stringIndex);
                state = 4;
            }
            else if(current_character == 0x0D) {
                state = 5;
                if (lineIndex < AT_COMMAND_MAX_LINES) {
                    data.data[lineIndex][stringIndex] = '\0';
                    stringIndex = 0;
                    lineIndex++;
                    data.line_count++;
                }
            }
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 5:{

            if(current_character == 0x0A) state = 6;
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 6:{

            if(current_character == 0x0D) state = 15;
            else if(current_character == '+') state = 3;
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 15:
        {
            if(current_character == 0x0A) state = 16;
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 16:
        {
            if(current_character == 'O') state = 12;
            else if(current_character == 'E') state = 7;
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 12:
        {
            if(current_character == 'K') {
                data.ok = true;
                state = 11;
            }
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 7:{

            if(current_character == 'R') {
                data.ok = false;
                state = 8;
            }
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 8:{

            if(current_character == 'R') state = 9;
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 9:{

            if(current_character == 'O') state = 10;
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 10:
        {
            if(current_character == 'R') state = 11;
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 11:
        {
            if(current_character ==  0x0D) state = 13;
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 13:
        {
            if(current_character ==  0x0A) state = 14;
            else {
                state = lineIndex = stringIndex = 0; resetMemory();
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        case 14:
        {
            state = lineIndex = stringIndex = 0; resetMemory();
            return STATE_MACHINE_READY_OK;
            break;
        }
        default:
            perror("State doesn't exist\n");
    }
    return STATE_MACHINE_NOT_READY;
}

