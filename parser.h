#ifndef _PARSER_H
#define _PARSER_H

#include <stdint.h>
#include <stdbool.h>

#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_MAX_LINE_SIZE 128


typedef struct
{
    uint8_t ok;
    uint8_t data[AT_COMMAND_MAX_LINES][AT_COMMAND_MAX_LINE_SIZE + 1];
    uint32_t line_count;
} AT_COMMAND_DATA;

typedef enum {
    STATE_MACHINE_NOT_READY,
    STATE_MACHINE_READY_OK,
    STATE_MACHINE_READY_WITH_ERROR
} STATE_MACHINE_RETURN_VALUE;

const char* getStringFromStateMachine(STATE_MACHINE_RETURN_VALUE ret);

STATE_MACHINE_RETURN_VALUE at_command_parser(uint8_t current_character);

#endif
