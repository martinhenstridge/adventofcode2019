#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static bool
analyse(uint password, bool *gt1, bool *eq2) {
    uint original = password;
    uint run = 1;
    bool local_gt1 = false;
    bool local_eq2 = false;

    uint prev = 10;
    uint curr = 10;

    for (uint i = 0; i < 6; i++) {
        curr = password % 10;
        password /= 10;

        if (curr > prev) {
            // Decreasing: unacceptable
            return false;
        } else if (curr == prev) {
            // Repeating
            ++run;
            local_gt1 = true;
        } else {
            // Increasing
            if (run == 2) {
                local_eq2 = true;
            }
            run = 1;
        }
        prev = curr;
    }

    if (run == 2) {
        local_eq2 = true;
    }

    *gt1 = local_gt1;
    *eq2 = local_eq2;
    return true;
}


void
solve04(const char *input)
{
    char *copy = strdup(input);

    char *lower_str = strtok(copy, "-");
    char *upper_str = strtok(NULL, "\n");

    uint lower = strtoul(lower_str, NULL, 10);
    uint upper = strtoul(upper_str, NULL, 10);

    uint count1 = 0;
    uint count2 = 0;

    bool gt1 = false;
    bool eq2 = false;

    for (uint password = lower; password <= upper; password++) {
        if (analyse(password, &gt1, &eq2)) {
            if (gt1) {
                ++count1;
                if (eq2) {
                    ++count2;
                }
            }
        }
    }

    printf("[04/1] %u\n", count1);
    printf("[04/2] %u\n", count2);
}
