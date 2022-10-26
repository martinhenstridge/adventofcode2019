#include <stdlib.h>
#include "util.h"

void solve01(const char *input);
void solve02(const char *input);
void solve03(const char *input);


int
main(int argc, char **argv)
{
    char *input;

    input = read_file("data/01.txt");
    solve01(input);
    free(input);

    input = read_file("data/02.txt");
    solve02(input);
    free(input);

    input = read_file("data/03.txt");
    solve03(input);
    free(input);

    return EXIT_SUCCESS;
}
