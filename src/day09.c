#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "intcode.h"


static int64_t
run_boost_program(IntcodeProgram *program, int64_t input)
{
    int64_t keycode;

    intcode_reset(program);
    intcode_send_input(program, input);

    intcode_run(program);
    assert(program->status == STATUS_OUTPUT_AVAILABLE);
    keycode = program->output;

    intcode_run(program);
    assert(program->status == STATUS_COMPLETE);

    return keycode;
}


void
solve09(const char *input)
{
    IntcodeProgram program = intcode_create(input);

    int64_t keycode = run_boost_program(&program, 1);
    int64_t coords = run_boost_program(&program, 2);

    printf("[09/1] %li\n", keycode);
    printf("[09/2] %li\n", coords);
}
