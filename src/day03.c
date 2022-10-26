#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE (1 << 20)
#define HASH_MASK (HASH_SIZE - 1)


typedef struct {
    int x;
    int y;
    uint hash;
    uint count;
} HashEntry;


static inline uint
calculate_hash(int x, int y)
{
    return 7919 * x + 3533 * y;
}


static HashEntry *
hash_create(void)
{
    HashEntry *table = calloc(HASH_SIZE, sizeof(*table));
    assert(table != NULL);
    return table;
}


static void
hash_insert(HashEntry *table, int x, int y, uint count)
{
    uint hash = calculate_hash(x, y);
    uint index = hash & HASH_MASK;

    // Find an empty slot in the table.
    while (table[index].count != 0) {
        ++index;
        if (index == HASH_SIZE) {
            index = 0;
        }
    }

    // Insert the entry.
    table[index].x = x;
    table[index].y = y;
    table[index].hash = hash;
    table[index].count = count;
}


static bool
hash_lookup(HashEntry *table, int x, int y, HashEntry *result)
{
    uint hash = calculate_hash(x, y);
    uint index = hash & HASH_MASK;

    // We're at the right place, but there's nothing here.
    if (table[index].count == 0) {
        return false;
    }

    // Skip until we hit an entry with the right hash value, or an empty
    // slot.
    while (table[index].count != 0 && table[index].hash != hash) {
        ++index;
        if (index == HASH_SIZE) {
            index = 0;
        }
    }

    // Check for matching data on each entry until we run out of entries
    // with the right hash value.
    while (table[index].count != 0 && table[index].hash == hash) {
        if (table[index].x == x && table[index].y == y) {
            *result = table[index];
            return true;
        }
        ++index;
        if (index == HASH_SIZE) {
            index = 0;
        }
    }

    // No more entries with the right hash value.
    return false;
}


static inline void
get_dx_dy(char direction, int *dx, int *dy)
{
    switch (direction) {
    case 'U':
        *dx = 0; *dy = +1;
        return;
    case 'D':
        *dx = 0; *dy = -1;
        return;
    case 'L':
        *dx = -1; *dy = 0;
        return;
    case 'R':
        *dx = +1; *dy = 0;
        return;
    default:
        assert(false);
    }
}


static void
insert_segments(HashEntry *table, char *wire)
{
    int x = 0;
    int y = 0;
    uint count = 0;

    for (char *segment = strtok(wire, ","); segment != NULL; segment = strtok(NULL, ",")) {
        int dx, dy;
        get_dx_dy(segment[0], &dx, &dy);

        int distance = strtol(segment + 1, NULL, 10);
        for (int i = 0; i < distance; i++) {
            x += dx;
            y += dy;
            ++count;
            hash_insert(table, x, y, count);
        }
    }
}


static void
find_intersections(HashEntry *table, char *wire, uint *out_manhattan, uint *out_combined)
{
    uint shortest_manhattan = 0xffffffff;
    uint shortest_combined = 0xffffffff;
    int x = 0;
    int y = 0;
    uint count = 0;

    for (char *segment = strtok(wire, ","); segment != NULL; segment = strtok(NULL, ",")) {
        int dx, dy;
        get_dx_dy(segment[0], &dx, &dy);

        int distance = strtol(segment + 1, NULL, 10);
        for (int i = 0; i < distance; i++) {
            HashEntry entry;

            x += dx;
            y += dy;
            ++count;
            if (hash_lookup(table, x, y, &entry)) {
                uint manhattan = abs(x) + abs(y);
                if (manhattan < shortest_manhattan) {
                    shortest_manhattan = manhattan;
                }
                uint combined = count + entry.count;
                if (combined < shortest_combined) {
                    shortest_combined = combined;
                }
            }
        }
    }

    *out_manhattan = shortest_manhattan;
    *out_combined = shortest_combined;
}


void
solve03(const char *input)
{
    uint manhattan, combined;

    char *copy = strdup(input);
    char *wire1 = strtok(copy, "\n");
    char *wire2 = strtok(NULL, "\n");

    HashEntry *table = hash_create();

    insert_segments(table, wire1);
    find_intersections(table, wire2, &manhattan, &combined);

    free(table);
    free(copy);

    printf("[03/1] %u\n", manhattan);
    printf("[03/2] %u\n", combined);
}
