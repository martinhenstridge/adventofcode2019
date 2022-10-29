#ifndef __INTCODE_H__
#define __INTCODE_H__

#include <stdlib.h>

typedef enum {
    OPCODE_ADD           = 1,
    OPCODE_MULTIPLY      = 2,
    OPCODE_INPUT         = 3,
    OPCODE_OUTPUT        = 4,
    OPCODE_JUMP_IF_TRUE  = 5,
    OPCODE_JUMP_IF_FALSE = 6,
    OPCODE_LESS_THAN     = 7,
    OPCODE_EQUALS        = 8,
    OPCODE_EXIT          = 99,
} Opcode;

typedef enum {
    MODE_POSITION = 0,
    MODE_IMMEDIATE = 1,
} ParameterMode;

typedef struct {
    size_t length;
    int *instructions;
    int *memory;
    int input;
    int output;
} IntcodeProgram;

IntcodeProgram read_program(const char *input);
void init_program(IntcodeProgram *program);
void run_program(IntcodeProgram *program);
void dump_program(IntcodeProgram *program);

#endif
