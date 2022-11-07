#ifndef __INTCODE_H__
#define __INTCODE_H__

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    STATUS_READY = 0,
    STATUS_COMPLETE = 1,
    STATUS_INPUT_REQUIRED = 2,
    STATUS_OUTPUT_AVAILABLE = 3,
} IntcodeStatus;

typedef struct Input_ {
    struct Input_ *next;
    int64_t value;
} IntcodeInput;

typedef struct {
    IntcodeStatus status;
    size_t length;
    size_t ip;
    size_t rb;
    int64_t *instructions;
    int64_t *memory;
    IntcodeInput *input;
    int64_t output;
} IntcodeProgram;

IntcodeProgram intcode_create(const char *input);
IntcodeProgram intcode_copy(IntcodeProgram *program);
void intcode_reset(IntcodeProgram *program);
void intcode_run(IntcodeProgram *program);
void intcode_send_input(IntcodeProgram *program, int64_t value);
void intcode_dump(IntcodeProgram *program);

#endif
