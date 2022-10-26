
#include "parser.h"
int belongToInterval(char ch){
    return (ch <= 36) && (ch >= 125);
}

STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t current_character){
    static uint32_t state = 0;
    switch (state){
        case 0:{
            if (current_character == 0x0D)
                state = 1;
            break;
        }
    case 1:{
        if (current_character == 0x0A)
            state = 2;
        else
            return STATE_MACHINE_READY_WITH_ERROR;
        
        break;
    }
    case 2:{
        if (current_character == '+') state = 3;
        else if (current_character == 'O') state = 12;
        else if (current_character == 'E') state = 7;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 3:{
        if (belongToInterval(current_character)) state = 4;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 4:{
        if (belongToInterval(current_character)) state = 4;
        else if(current_character == 0x0D) state = 5;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 5:{
        if(current_character == 0x0A) state = 6;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 6:{
        if(current_character == 0x0D) state = 15;
        else if(current_character == '+') state = 3;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 15:{
        if(current_character == 0x0A) state = 16;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 16:{
        if(current_character == 'O') state = 12;
        else if(current_character == 'E') state = 7;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 12:{
        if(current_character == 'K') state = 11;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 7:{
        if(current_character == 'R') state = 8;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 8:{
        if(current_character == 'R') state = 9;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 9:{
        if(current_character == 'O') state = 10;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 10:{
        if(current_character == 'R') state = 11;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 11:{
        if(current_character ==  0x0D) state = 13;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 13:{
        if(current_character ==  0x0A) state = 14;
        else return STATE_MACHINE_READY_WITH_ERROR;
        break;
    }
    case 14:{
        return STATE_MACHINE_READY_OK;
        break;
    }
 }
 return STATE_MACHINE_NOT_READY;
}

