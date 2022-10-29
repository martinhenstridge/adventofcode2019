#include <stdio.h>
#include <stdlib.h>

#include "intcode.h"


void
solve05(const char *input)
{
    IntcodeProgram program = read_program(input);

    int diagnostic1;
    int diagnostic5;

    init_program(&program);
    program.input = 1;
    run_program(&program);
    diagnostic1 = program.output;

    init_program(&program);
    program.input = 5;
    run_program(&program);
    diagnostic5 = program.output;

    printf("[05/1] %i\n", diagnostic1);
    printf("[05/2] %i\n", diagnostic5);
}
