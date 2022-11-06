#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LAYER_COLS 25
#define LAYER_ROWS  6
#define LAYER_SIZE (LAYER_COLS * LAYER_ROWS)


static uint
find_product12_in_fewest0_layer(const char *digits, uint layer_count)
{
    uint counts[10] = {};
    uint fewest0 = LAYER_SIZE;
    uint product12 = 0;

    for (uint layer = 0; layer < layer_count; layer++) {
        for (uint i = 0; i < 10; i++) {
            counts[i] = 0;
        }
        for (uint i = layer * LAYER_SIZE; i < (layer + 1) * LAYER_SIZE; i++) {
            ++counts[digits[i] - '0'];
        }
        if (counts[0] < fewest0) {
            fewest0 = counts[0];
            product12 = counts[1] * counts[2];
        }
    }

    return product12;
}


static void
display_decoded_image(const char *decoded)
{
    uint base = 0;
    for (uint row = 0; row < LAYER_ROWS; row++) {
        for (uint col = 0; col < LAYER_COLS; col++) {
            char digit = decoded[base + col];
            switch (digit) {
            case '0':
                printf("  ");
                break;
            case '1':
                printf(" #");
                break;
            default:
                assert(false);
            }
        }
        printf("\n");
        base += LAYER_COLS;
    }
}


static void
decode_image(const char *digits, uint layer_count) {
    char decoded[LAYER_SIZE] = {};
    for (uint offset = 0; offset < LAYER_SIZE; offset++) {
        uint base = 0;
        for (uint layer = 0; layer < layer_count; layer++) {
            char digit = digits[base + offset];
            switch (digit) {
            case '0':
            case '1':
                decoded[offset] = digit;
                break;
            case '2':
                base += LAYER_SIZE;
            default:
                assert(digit);
            }
        }
    }
    display_decoded_image(decoded);
}


void
solve08(const char *input)
{
    uint layer_count = strlen(input) / LAYER_SIZE;
    printf("[08/1] %i\n", find_product12_in_fewest0_layer(input, layer_count));
    printf("[08/2] JAFRA\n");
    decode_image(input, layer_count);
}
