#include <stdlib.h>
#include "day01.h"
#include "util.h"

int
main(int argc, char **argv)
{
    char *input01 = read_file("data/01.txt");
    solve01(input01);
    free(input01);

    return EXIT_SUCCESS;
}
