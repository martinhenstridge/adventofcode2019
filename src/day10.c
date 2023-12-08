#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t rows = 0;
static size_t cols = 0;


static inline bool
asteroid(const char *map, size_t row, size_t col)
{
    return map[col + (cols + 1) * row] == '#';
}


static int
gcd(int a, int b)
{
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}


static size_t
count_visible(const char *map, size_t row, size_t col)
{
    size_t count = 0;

    for (int dr = -row; dr < (int)(rows - row); dr++) {
        for (int dc = -col; dc < (int)(cols - col); dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            if (gcd(abs(dr), abs(dc)) != 1) {
                continue;
            }

            size_t r = row + dr;
            size_t c = col + dc;
            while (r >= 0 && r < rows && c >= 0 && c < cols) {
                if (asteroid(map, r, c)) {
                    ++count;
                    break;
                }
                r += dr;
                c += dc;
            }
        }
    }

    return count;
}


const char *INPUT =
".#..#\n"
".....\n"
"#####\n"
"....#\n"
"...##\n";

void
solve10(const char *input)
{
    const char *map = input;
    size_t max_count = 0;

    cols = (size_t)strstr(map, "\n") - (size_t)map;
    rows = strlen(map) / (cols + 1);

    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            if (!asteroid(map, r, c)) {
                continue;
            }
            size_t count = count_visible(map, r, c);
            if (count > max_count) {
                max_count = count;
            }
        }
    }

    printf("[10/1] %li\n", max_count);
    printf("[10/2] %li\n", cols);
}
