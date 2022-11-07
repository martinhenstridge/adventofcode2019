#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "intcode.h"


static int64_t
run_to_completion(IntcodeProgram *program, int64_t input)
{
    intcode_reset(program);
    intcode_send_input(program, input);
    while (program->status != STATUS_COMPLETE) {
        intcode_run(program);
    }

    return program->output;
}


void
solve05(const char *input)
{
    IntcodeProgram program = intcode_create(input);

    int64_t diagnostic1 = run_to_completion(&program, 1);
    int64_t diagnostic5 = run_to_completion(&program, 5);

    printf("[05/1] %li\n", diagnostic1);
    printf("[05/2] %li\n", diagnostic5);
}
