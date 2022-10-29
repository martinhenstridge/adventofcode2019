#include <stdlib.h>
#include "util.h"

void solve01(const char *input);
void solve02(const char *input);
void solve03(const char *input);
void solve04(const char *input);
void solve05(const char *input);


typedef void (*Solver)(const char *);

typedef struct {
    Solver solver;
    char *path;
} Day;

static const Day days[] = {
    { solve01, "data/01.txt" },
    { solve02, "data/02.txt" },
    { solve03, "data/03.txt" },
    { solve04, "data/04.txt" },
    { solve05, "data/05.txt" },
};

static const size_t num_days = sizeof(days) / sizeof(*days);


int
main(int argc, char **argv)
{
    char *input;

    for (size_t day = 0; day < num_days; day++) {
        input = read_file(days[day].path);
        days[day].solver(input);
        free(input);
    }

    return EXIT_SUCCESS;
}
