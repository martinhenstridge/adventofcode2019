#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    size_t length;
    int *instructions;
} Program;


static Program
read_program(const char *input)
{
    static size_t length = 0;
    static int instructions[256];
    Program program;

    char *tokstr = strdup(input);
    for (char *tok = strtok(tokstr, ","); tok != NULL; tok = strtok(NULL, ",")) {
        instructions[length++] = strtol(tok, NULL, 10);
    }
    free(tokstr);

    program.length = length;
    program.instructions = instructions;

    return program;
}


static int *
copy_instructions(Program program)
{
    size_t size = sizeof(int) * program.length;
    int *instructions = malloc(size);
    assert(instructions != NULL);

    memcpy(instructions, program.instructions, size);
    return instructions;
}


static int
run_program(Program program, int noun, int verb)
{
    int *memory = copy_instructions(program);
    int output;

    memory[1] = noun;
    memory[2] = verb;

    size_t ptr = 0;
    while (true) {
        switch (memory[ptr]) {
        case 1:
            {
                int src1 = memory[ptr + 1];
                int src2 = memory[ptr + 2];
                int dst = memory[ptr + 3];
                memory[dst] = memory[src1] + memory[src2];
            }
            ptr += 4;
            break;
        case 2:
            {
                int src1 = memory[ptr + 1];
                int src2 = memory[ptr + 2];
                int dst = memory[ptr + 3];
                memory[dst] = memory[src1] * memory[src2];
            }
            ptr += 4;
            break;
        case 99:
            output = memory[0];
            free(memory);
            return output;
        default:
            assert(false);
        }
    }
}


static int
search_output(Program program, int goal)
{
    int output;
    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            output = run_program(program, noun, verb);
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
    Program program = read_program(input);

    printf("[02/1] %i\n", run_program(program, 12, 2));
    printf("[02/2] %i\n", search_output(program, 19690720));
}
