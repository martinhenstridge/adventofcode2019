#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "intcode.h"


static int64_t
run_linear(IntcodeProgram *program, int64_t *settings)
{
    int64_t output = 0;

    for (size_t i = 0; i < 5; i++) {
        intcode_reset(program);
        intcode_send_input(program, settings[i]);
        intcode_send_input(program, output);
        intcode_run(program);
        output = program->output;
    }

    return output;
}


static int64_t
run_feedback(IntcodeProgram *amplifiers, int64_t *settings)
{
    int64_t output = 0;
    int64_t result = 0;

    for (uint i = 0; i < 5; i++) {
        intcode_reset(&amplifiers[i]);
        intcode_send_input(&amplifiers[i], settings[i]);
    }

    uint i = 0;
    while (true) {
        intcode_run(&amplifiers[i]);
        switch (amplifiers[i].status) {
        case STATUS_COMPLETE:
            return result;
        case STATUS_INPUT_REQUIRED:
            intcode_send_input(&amplifiers[i], output);
            break;
        case STATUS_OUTPUT_AVAILABLE:
            output = amplifiers[i].output;
            if (++i == 5) {
                i = 0;
                result = output;
            }
            break;
        default:
            assert(false);
        }
    }
}


static int64_t
find_max_linear(IntcodeProgram *program)
{
    int64_t max = 0;
    int64_t output;
    int64_t settings[5];

    for (uint a = 0; a < 5; a++) {
        for (uint b = 0; b < 5; b++) {
            if (b == a) continue;
            for (uint c = 0; c < 5; c++) {
                if (c == a || c == b) continue;
                for (uint d = 0; d < 5; d++) {
                    if (d == a || d == b || d == c) continue;
                    for (uint e = 0; e < 5; e++) {
                        if (e == a || e == b || e == c || e == d) continue;
                        settings[0] = a;
                        settings[1] = b;
                        settings[2] = c;
                        settings[3] = d;
                        settings[4] = e;
                        output = run_linear(program, settings);
                        if (output > max) {
                            max = output;
                        }
                    }
                }
            }
        }
    }

    return max;
}


static int64_t
find_max_feedback(IntcodeProgram *program)
{
    int64_t max = 0;
    int64_t output;
    int64_t settings[5];
    IntcodeProgram amplifiers[5];

    for (uint i = 0; i < 5; i++) {
        amplifiers[i] = intcode_copy(program);
    }

    for (uint a = 5; a < 10; a++) {
        for (uint b = 5; b < 10; b++) {
            if (b == a) continue;
            for (uint c = 5; c < 10; c++) {
                if (c == a || c == b) continue;
                for (uint d = 5; d < 10; d++) {
                    if (d == a || d == b || d == c) continue;
                    for (uint e = 5; e < 10; e++) {
                        if (e == a || e == b || e == c || e == d) continue;
                        settings[0] = a;
                        settings[1] = b;
                        settings[2] = c;
                        settings[3] = d;
                        settings[4] = e;
                        output = run_feedback(amplifiers, settings);
                        if (output > max) {
                            max = output;
                        }
                    }
                }
            }
        }
    }

    return max;
}


void
solve07(const char *input)
{
    IntcodeProgram program = intcode_create(input);

    printf("[07/1] %li\n", find_max_linear(&program));
    printf("[07/2] %li\n", find_max_feedback(&program));
}
