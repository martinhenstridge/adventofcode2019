#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"


static int
fuel_requirement(int mass)
{
    return (mass / 3) - 2;
}


static int
extra_fuel_requirement(int mass)
{
    int total = 0;
    while (true) {
        mass = fuel_requirement(mass);
        if (mass > 0) {
            total += mass;
        } else {
            break;
        }
    }
    return total;
}


int
main(int argc, char **argv)
{
    char *input = read_file("data/01.txt");
    int mass, total;
    char *tokstr;

    total = 0;
    tokstr = strdup(input);
    for (char *tok = strtok(tokstr, "\n"); tok != NULL; tok = strtok(NULL, "\n")) {
        mass = strtol(tok, NULL, 10);
        total += fuel_requirement(mass);
    }
    printf("[01/1] %i\n", total);
    free(tokstr);

    total = 0;
    tokstr = strdup(input);
    for (char *tok = strtok(tokstr, "\n"); tok != NULL; tok = strtok(NULL, "\n")) {
        mass = strtol(tok, NULL, 10);
        total += extra_fuel_requirement(mass);
    }
    printf("[02/1] %i\n", total);
    free(tokstr);

    free(input);
}
