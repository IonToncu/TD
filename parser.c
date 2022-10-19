
#include "parser.h"

STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t current_character)
{
    static uint32_t state = 0;
    switch (state)
    {
    case 0:
    {
        if (current_character == 0x0D)
        {
            state = 1;
        }
        break;
    }
    case 1:
    {
        if (current_character == 0x0A)
        {
            state = 2;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }
 case 2:
 {
 if (current_character == 'O')
 {
 state = 3;
 }
 else
 {
 return STATE_MACHINE_READY_WITH_ERROR;
 }
 }
 }
 return STATE_MACHINE_NOT_READY;
}