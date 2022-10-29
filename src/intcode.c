#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intcode.h"


static Opcode
decode_opcode(int encoded, ParameterMode *modes)
{
    assert(encoded > 0);
    uint digits = (uint)encoded;

    Opcode opcode = digits % 100;
    digits /= 100;

    modes[0] = digits % 10;
    digits /= 10;

    modes[1] = digits % 10;
    digits /= 10;

    modes[2] = digits % 10;
    digits /= 10;

    return opcode;
}


static size_t
get_param_addr(ParameterMode mode, int *memory, size_t ptr)
{
    switch (mode) {
    case MODE_POSITION:
        return memory[ptr];
    case MODE_IMMEDIATE:
        return ptr;
    default:
        printf("Bad parameter mode: %u\n", mode);
        assert(false);
    }
}


IntcodeProgram
read_program(const char *input)
{
    IntcodeProgram program;

    size_t ccount = 1;
    size_t icount = 1;
    size_t i = 0;
    while (input[i] != '\0') {
        ++ccount;
        if (input[i] == ',') {
            ++icount;
        }
        ++i;
    }

    program.length = icount;
    program.instructions = malloc(sizeof(int) * icount);
    assert(program.instructions != NULL);
    program.memory = malloc(sizeof(int) * icount);
    assert(program.memory != NULL);

    char *copy = malloc(ccount);
    memcpy(copy, input, ccount);
    i = 0;
    for (char *tok = strtok(copy, ","); tok != NULL; tok = strtok(NULL, ",")) {
        program.instructions[i++] = strtol(tok, NULL, 10);
    }
    free(copy);

    return program;
}


void
init_program(IntcodeProgram *program)
{
    memcpy(program->memory, program->instructions, sizeof(int) * program->length);
}


void
run_program(IntcodeProgram *program)
{
    int output;
    int *memory = program->memory;
    size_t ip = 0;

    Opcode opcode;
    ParameterMode modes[3];

    while (true) {
        opcode = decode_opcode(memory[ip], modes);

        switch (opcode) {
        case OPCODE_ADD:
            {
                assert(modes[2] != MODE_IMMEDIATE);
                size_t p1 = get_param_addr(modes[0], memory, ip + 1);
                size_t p2 = get_param_addr(modes[1], memory, ip + 2);
                size_t p3 = get_param_addr(modes[2], memory, ip + 3);
                memory[p3] = memory[p1] + memory[p2];
                ip += 4;
            }
            break;

        case OPCODE_MULTIPLY:
            {
                assert(modes[2] != MODE_IMMEDIATE);
                size_t p1 = get_param_addr(modes[0], memory, ip + 1);
                size_t p2 = get_param_addr(modes[1], memory, ip + 2);
                size_t p3 = get_param_addr(modes[2], memory, ip + 3);
                memory[p3] = memory[p1] * memory[p2];
                ip += 4;
            }
            break;

        case OPCODE_INPUT:
            {
                assert(modes[0] != MODE_IMMEDIATE);
                size_t p1 = get_param_addr(modes[0], memory, ip + 1);
                memory[p1] = program->input;
                ip += 2;
            }
            break;

        case OPCODE_OUTPUT:
            {
                size_t p1 = get_param_addr(modes[0], memory, ip + 1);
                program->output = memory[p1];
                ip += 2;
            }
            break;

        case OPCODE_JUMP_IF_TRUE:
            {
                size_t p1 = get_param_addr(modes[0], memory, ip + 1);
                size_t p2 = get_param_addr(modes[1], memory, ip + 2);
                if (memory[p1] != 0) {
                    ip = memory[p2];
                } else {
                    ip += 3;
                }
            }
            break;

        case OPCODE_JUMP_IF_FALSE:
            {
                size_t p1 = get_param_addr(modes[0], memory, ip + 1);
                size_t p2 = get_param_addr(modes[1], memory, ip + 2);
                if (memory[p1] == 0) {
                    ip = memory[p2];
                } else {
                    ip += 3;
                }
            }
            break;

        case OPCODE_LESS_THAN:
            {
                assert(modes[2] != MODE_IMMEDIATE);
                size_t p1 = get_param_addr(modes[0], memory, ip + 1);
                size_t p2 = get_param_addr(modes[1], memory, ip + 2);
                size_t p3 = get_param_addr(modes[2], memory, ip + 3);
                if (memory[p1] < memory[p2]) {
                    memory[p3] = 1;
                } else {
                    memory[p3] = 0;
                }
                ip += 4;
            }
            break;

        case OPCODE_EQUALS:
            {
                assert(modes[2] != MODE_IMMEDIATE);
                size_t p1 = get_param_addr(modes[0], memory, ip + 1);
                size_t p2 = get_param_addr(modes[1], memory, ip + 2);
                size_t p3 = get_param_addr(modes[2], memory, ip + 3);
                if (memory[p1] == memory[p2]) {
                    memory[p3] = 1;
                } else {
                    memory[p3] = 0;
                }
                ip += 4;
            }
            break;

        case OPCODE_EXIT:
            return;

        default:
            printf("Bad opcode: %u\n", opcode);
            assert(false);
        }
    }
}


void
dump_program(IntcodeProgram *program)
{
    for (size_t i = 0; i < program->length; i++) {
        printf("[%zu] %i %i\n", i, program->instructions[i], program->memory[i]);
    }
}
