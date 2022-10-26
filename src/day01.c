#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int
fuel_requirement(int mass)
{
    return (mass / 3) - 2;
}


static int
recursive_fuel_requirement(int mass)
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


static int
total_naive(const char *input)
{
    int total = 0;
    int mass;

    char *tokstr = strdup(input);
    for (char *tok = strtok(tokstr, "\n"); tok != NULL; tok = strtok(NULL, "\n")) {
        mass = strtol(tok, NULL, 10);
        total += fuel_requirement(mass);
    }
    free(tokstr);

    return total;
}


static int
total_recursive(const char *input)
{
    int total = 0;
    int mass;

    char *tokstr = strdup(input);
    for (char *tok = strtok(tokstr, "\n"); tok != NULL; tok = strtok(NULL, "\n")) {
        mass = strtol(tok, NULL, 10);
        total += recursive_fuel_requirement(mass);
    }
    free(tokstr);

    return total;
}


void
solve01(const char *input)
{
    printf("[01/1] %i\n", total_naive(input));
    printf("[01/2] %i\n", total_recursive(input));
}
