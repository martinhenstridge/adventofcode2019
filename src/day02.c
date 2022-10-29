#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "intcode.h"


static int
run_noun_verb(IntcodeProgram *program, int noun, int verb)
{
    init_program(program);
    program->memory[1] = noun;
    program->memory[2] = verb;

    run_program(program);
    return program->memory[0];
}


static int
search_noun_verb(IntcodeProgram *program, int goal)
{
    int output;
    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            output = run_noun_verb(program, noun, verb);
            if (output == goal) {
                return 100 * noun + verb;
            }
        }
    }
    assert(false);
}


void
solve02(const char *input)
{
    IntcodeProgram program = read_program(input);

    printf("[02/1] %i\n", run_noun_verb(&program, 12, 2));
    printf("[02/2] %i\n", search_noun_verb(&program, 19690720));
}
