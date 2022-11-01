#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "intcode.h"


static int
run_to_completion(IntcodeProgram *program, int input)
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

    int diagnostic1 = run_to_completion(&program, 1);
    int diagnostic5 = run_to_completion(&program, 5);

    printf("[05/1] %i\n", diagnostic1);
    printf("[05/2] %i\n", diagnostic5);
}
