#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMELEN 4


typedef struct HashEntry_{
    struct HashEntry_ *parent;
    struct HashEntry_ *child1;
    struct HashEntry_ *child2;
    char name[NAMELEN];
    uint count;
    uint distance;
} HashEntry;


static inline uint
calculate_hash(const char *name)
{
    /*
     * - Names are constructed from 3 characters in the range 0-Z.
     * - There are 'Z' - '0' == 42 possible values for each character.
     * - Each character can therefore be represented by a 6-bit value.
     * - Each name can therefore be represented by an 18-bit value.
     * - A hash table with 2^18 buckets can therefore accommodate all
     *   3-character names without any collisions.
     */
    return (name[0] - '0') << 0
         | (name[1] - '0') << 6
         | (name[2] - '0') << 12;
}


static HashEntry *
hash_lookup(HashEntry *table, const char *name)
{
    uint index = calculate_hash(name);
    return &table[index];
}


static HashEntry *
parse_orbits(const char *input)
{
    HashEntry *table = calloc(1 << 18, sizeof(*table));
    char *copy = strdup(input);
    char *orbit = NULL;
    char *ctx = NULL;

    for (orbit = strtok_r(copy, "\n", &ctx);
         orbit != NULL;
         orbit = strtok_r(NULL, "\n", &ctx)) {
        char *name_orbitee = strtok(orbit, ")");
        char *name_orbiter = strtok(NULL, ")");

        HashEntry *orbitee = hash_lookup(table, name_orbitee);
        HashEntry *orbiter = hash_lookup(table, name_orbiter);

        strncpy(orbitee->name, name_orbitee, NAMELEN);
        strncpy(orbiter->name, name_orbiter, NAMELEN);

        orbiter->parent = orbitee;
        if (orbitee->child1 == NULL) {
            orbitee->child1 = orbiter;
        } else {
            orbitee->child2 = orbiter;
        }
    }

    free(copy);
    return table;
}


static uint
count_orbits(HashEntry *orbits)
{
    uint count = 0;
    HashEntry *curr = hash_lookup(orbits, "COM");
    HashEntry *next = NULL;
    HashEntry *prev = NULL;

    while (curr != NULL) {
        if (prev == curr->parent && curr->child1 != NULL) {
            next = curr->child1;
            next->count = curr->count + 1;
            count += next->count;
        } else if (prev == curr->child1 && curr->child2 != NULL) {
            next = curr->child2;
            next->count = curr->count + 1;
            count += next->count;
        } else {
            next = curr->parent;
        }
        prev = curr;
        curr = next;
    }

    return count;
}


static uint
count_transfers(HashEntry *orbits)
{
    uint distance;
    HashEntry *node;

    // Walk "inwards" from YOU, labelling each with distance travelled.
    distance = 0;
    for (node = hash_lookup(orbits, "YOU");
         node != NULL;
         node = node->parent) {
        node->distance = distance++;
    }

    // Walk "inwards" from SAN until encountering an ancestor of YOU.
    distance = 0;
    for (node = hash_lookup(orbits, "SAN");
         node->distance == 0;
         node = node->parent) {
        ++distance;
    }

    // The distance is the distance from YOU + distance from SAN,
    // omitting the first hop from each.
    return distance + node->distance - 2;
}

void
solve06(const char *input)
{
    HashEntry *orbits = parse_orbits(input);

    printf("[06/1] %i\n", count_orbits(orbits));
    printf("[06/2] %i\n", count_transfers(orbits));

    free(orbits);
}
