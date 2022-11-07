#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "intcode.h"


static int64_t
run_noun_verb(IntcodeProgram *program, int64_t noun, int64_t verb)
{
    intcode_reset(program);
    program->memory[1] = noun;
    program->memory[2] = verb;

    intcode_run(program);
    return program->memory[0];
}


static int64_t
search_noun_verb(IntcodeProgram *program, int64_t goal)
{
    int64_t output;
    for (int64_t noun = 0; noun < 100; noun++) {
        for (int64_t verb = 0; verb < 100; verb++) {
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
    IntcodeProgram program = intcode_create(input);

    printf("[02/1] %li\n", run_noun_verb(&program, 12, 2));
    printf("[02/2] %li\n", search_noun_verb(&program, 19690720));
}
