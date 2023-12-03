#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intcode.h"

#define MEMORY_SIZE (1 << 16)


typedef enum {
    OPCODE_ADD            =  1,
    OPCODE_MULTIPLY       =  2,
    OPCODE_INPUT          =  3,
    OPCODE_OUTPUT         =  4,
    OPCODE_JUMP_IF_TRUE   =  5,
    OPCODE_JUMP_IF_FALSE  =  6,
    OPCODE_LESS_THAN      =  7,
    OPCODE_EQUALS         =  8,
    OPCODE_ADJUST_RELBASE =  9,
    OPCODE_EXIT           = 99,
} Opcode;


typedef enum {
    MODE_POSITION = 0,
    MODE_IMMEDIATE = 1,
    MODE_RELATIVE = 2,
} ParameterMode;


static Opcode
decode_opcode(int64_t encoded, ParameterMode *modes)
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
decode_address(ParameterMode mode, int64_t *memory, size_t base, size_t ptr)
{
    switch (mode) {
    case MODE_POSITION:
        return memory[ptr];
    case MODE_IMMEDIATE:
        return ptr;
    case MODE_RELATIVE:
        return memory[ptr] + base;
    default:
        printf("Bad parameter mode: %u\n", mode);
        assert(false);
    }
}


static void
enqueue_input(IntcodeProgram *program, IntcodeInput *input)
{
    IntcodeInput **entry = &program->input;
    while (*entry != NULL) {
        entry = &(*entry)->next;
    }
    *entry = input;
}


static IntcodeInput *
dequeue_input(IntcodeProgram *program)
{
    IntcodeInput *input;

    if (program->input == NULL) {
        return NULL;
    }

    input = program->input;
    program->input = input->next;
    input->next = NULL;

    return input;
}


IntcodeProgram
intcode_create(const char *input)
{
    IntcodeProgram program = {};

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
    program.instructions = malloc(sizeof(int64_t) * icount);
    assert(program.instructions != NULL);
    program.memory = malloc(sizeof(int64_t) * MEMORY_SIZE);
    assert(program.memory != NULL);

    char *copy = malloc(ccount);
    memcpy(copy, input, ccount);
    i = 0;
    for (char *tok = strtok(copy, ","); tok != NULL; tok = strtok(NULL, ",")) {
        program.instructions[i++] = strtoll(tok, NULL, 10);
    }
    free(copy);

    return program;
}


IntcodeProgram
intcode_copy(IntcodeProgram *program)
{
    IntcodeProgram copy = {};
    size_t copy_bytes;

    copy.length = program->length;
    copy_bytes = sizeof(int64_t) * copy.length;

    copy.instructions = malloc(copy_bytes);
    assert(copy.instructions != NULL);
    memcpy(copy.instructions, program->instructions, copy_bytes);

    copy.memory = malloc(sizeof(int64_t) * MEMORY_SIZE);
    assert(copy.memory != NULL);

    return copy;
}


void
intcode_reset(IntcodeProgram *program)
{
    program->status = STATUS_READY;
    memset(program->memory, 0, sizeof(int64_t) * MEMORY_SIZE);
    memcpy(program->memory,
           program->instructions,
           sizeof(int64_t) * program->length);
    while (program->input != NULL) {
        free(dequeue_input(program));
    }
    program->input = NULL;
    program->ip = 0;
    program->rb = 0;
}


void
intcode_run(IntcodeProgram *program)
{
    int64_t *memory = program->memory;
    size_t rb;
    size_t ip;

    Opcode opcode;
    ParameterMode modes[3];

    while (true) {
        ip = program->ip;
        rb = program->rb;
        opcode = decode_opcode(memory[ip], modes);

        switch (opcode) {
        case OPCODE_ADD:
            {
                assert(modes[2] != MODE_IMMEDIATE);
                size_t a1 = decode_address(modes[0], memory, rb, ip + 1);
                size_t a2 = decode_address(modes[1], memory, rb, ip + 2);
                size_t a3 = decode_address(modes[2], memory, rb, ip + 3);
                memory[a3] = memory[a1] + memory[a2];
                program->ip += 4;
            }
            break;

        case OPCODE_MULTIPLY:
            {
                assert(modes[2] != MODE_IMMEDIATE);
                size_t a1 = decode_address(modes[0], memory, rb, ip + 1);
                size_t a2 = decode_address(modes[1], memory, rb, ip + 2);
                size_t a3 = decode_address(modes[2], memory, rb, ip + 3);
                memory[a3] = memory[a1] * memory[a2];
                program->ip += 4;
            }
            break;

        case OPCODE_INPUT:
            {
                assert(modes[0] != MODE_IMMEDIATE);
                IntcodeInput *input;
                input = dequeue_input(program);
                if (input == NULL) {
                    program->status = STATUS_INPUT_REQUIRED;
                    return;
                }
                size_t a1 = decode_address(modes[0], memory, rb, ip + 1);
                memory[a1] = input->value;
                free(input);
                program->ip += 2;
            }
            break;

        case OPCODE_OUTPUT:
            {
                size_t a1 = decode_address(modes[0], memory, rb, ip + 1);
                program->output = memory[a1];
                program->ip += 2;
                program->status = STATUS_OUTPUT_AVAILABLE;
                return;
            }
            break;

        case OPCODE_JUMP_IF_TRUE:
            {
                size_t a1 = decode_address(modes[0], memory, rb, ip + 1);
                size_t a2 = decode_address(modes[1], memory, rb, ip + 2);
                if (memory[a1] != 0) {
                    program->ip = memory[a2];
                } else {
                    program->ip += 3;
                }
            }
            break;

        case OPCODE_JUMP_IF_FALSE:
            {
                size_t a1 = decode_address(modes[0], memory, rb, ip + 1);
                size_t a2 = decode_address(modes[1], memory, rb, ip + 2);
                if (memory[a1] == 0) {
                    program->ip = memory[a2];
                } else {
                    program->ip += 3;
                }
            }
            break;

        case OPCODE_LESS_THAN:
            {
                assert(modes[2] != MODE_IMMEDIATE);
                size_t a1 = decode_address(modes[0], memory, rb, ip + 1);
                size_t a2 = decode_address(modes[1], memory, rb, ip + 2);
                size_t a3 = decode_address(modes[2], memory, rb, ip + 3);
                if (memory[a1] < memory[a2]) {
                    memory[a3] = 1;
                } else {
                    memory[a3] = 0;
                }
                program->ip += 4;
            }
            break;

        case OPCODE_EQUALS:
            {
                assert(modes[2] != MODE_IMMEDIATE);
                size_t a1 = decode_address(modes[0], memory, rb, ip + 1);
                size_t a2 = decode_address(modes[1], memory, rb, ip + 2);
                size_t a3 = decode_address(modes[2], memory, rb, ip + 3);
                if (memory[a1] == memory[a2]) {
                    memory[a3] = 1;
                } else {
                    memory[a3] = 0;
                }
                program->ip += 4;
            }
            break;

        case OPCODE_ADJUST_RELBASE:
            {
                size_t a1 = decode_address(modes[0], memory, rb, ip + 1);
                program->rb += memory[a1];
                program->ip += 2;
            }
            break;

        case OPCODE_EXIT:
            program->status = STATUS_COMPLETE;
            return;

        default:
            printf("Bad opcode: %u\n", opcode);
            assert(false);
        }
    }
}


void
intcode_send_input(IntcodeProgram *program, int64_t value)
{
    IntcodeInput *input = malloc(sizeof(*input));
    assert(input != NULL);

    input->next = NULL;
    input->value = value;

    enqueue_input(program, input);
}


void
intcode_dump(IntcodeProgram *program)
{
    for (size_t i = 0; i < program->length; i++) {
        printf("[%zu] %li %li\n", i, program->instructions[i], program->memory[i]);
    }
}
