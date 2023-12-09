#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t r;
    size_t c;
} Point;

typedef struct {
    int dr;
    int dc;
    float angle;
} Ray;


static size_t rows = 0;
static size_t cols = 0;


static inline bool
asteroid(const char *map, Point p)
{
    return map[p.c + (cols + 1) * p.r] == '#';
}


static inline void
vaporise(char *map, Point p)
{
    map[p.c + (cols + 1) * p.r] = 'O';
}


static int
gcd(int a, int b)
{
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}


static inline bool
coprime(int a, int b)
{
    return gcd(abs(a), abs(b)) == 1;
}


static size_t
count_visible(const char *map, Point base)
{
    size_t count = 0;

    for (int dr = -base.r; dr < (int)(rows - base.r); dr++) {
        for (int dc = -base.c; dc < (int)(cols - base.c); dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            if (!coprime(dr, dc)) {
                continue;
            }

            Point p = {base.r + dr, base.c + dc};
            while (p.r >= 0 && p.r < rows && p.c >= 0 && p.c < cols) {
                if (asteroid(map, p)) {
                    ++count;
                    break;
                }
                p.r += dr;
                p.c += dc;
            }
        }
    }

    return count;
}


static float
calculate_angle(int dr, int dc)
{
    float angle;

    if (dr == 0) {
        return (dc > 0) ? (M_PI / 2) : (3 * M_PI / 2);
    }
    if (dc == 0) {
        return (dr < 0) ? 0 : M_PI;
    }

    /*
     *     |
     *   4 | 1
     * ----+----
     *   3 | 2
     *     |
     */

    angle = atanf(fabs(dc) / fabs(dr));

    // Quadrant 1
    if (dr < 0 && dc > 0) {
        return angle;
    }

    // Quadrant 2
    if (dr > 0 && dc > 0) {
        return M_PI - angle;
    }

    // Quadrant 3
    if (dr > 0 && dc < 0) {
        return M_PI + angle;
    }

    // Quadrant 4
    if (dr < 0 && dc < 0) {
        return 2 * M_PI - angle;
    }

    return INFINITY;
}


static int
compare_rays(const void *a, const void *b)
{
    Ray *ra = (Ray *)a;
    Ray *rb = (Ray *)b;
    if (ra->angle > rb->angle) {
        return +1;
    }
    if (ra->angle < rb->angle) {
        return -1;
    }
    return 0;
}


static size_t
find_ordered_rays(const char *map, Point base, Ray *rays)
{
    size_t count = 0;

    for (int dr = -base.r; dr < (int)(rows - base.r); dr++) {
        for (int dc = -base.c; dc < (int)(cols - base.c); dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            if (gcd(abs(dr), abs(dc)) != 1) {
                continue;
            }

            Point p = {base.r + dr, base.c + dc};
            while (p.r >= 0 && p.r < rows && p.c >= 0 && p.c < cols) {
                if (asteroid(map, p)) {
                    rays[count].dr = dr;
                    rays[count].dc = dc;
                    rays[count].angle = calculate_angle(dr, dc);
                    ++count;
                    break;
                }
                p.r += dr;
                p.c += dc;
            }
        }
    }

    qsort(rays, count, sizeof(Ray), compare_rays);
    return count;
}


static size_t
vaporise200(char *map, Point base, Ray *rays, size_t ray_count)
{
    size_t count = 0;

    for (size_t i = 0; i < ray_count; i++) {
        Point p = {base.r + rays[i].dr, base.c + rays[i].dc};
        while (p.r >= 0 && p.r < rows && p.c >= 0 && p.c < cols) {
            if (asteroid(map, p)) {
                vaporise(map, p);
                ++count;
                if (count == 200) {
                    return p.r + 100 * p.c;
                }
                break;
            }
            p.r += rays[i].dr;
            p.c += rays[i].dc;
        }
    }
}


void
solve10(const char *input)
{
    char *map = strdup(input);
    cols = (size_t)strstr(map, "\n") - (size_t)map;
    rows = strlen(map) / (cols + 1);

    size_t visible = 0;
    Point base = {};

    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            Point p = {r, c};
            if (!asteroid(map, p)) {
                continue;
            }
            size_t count = count_visible(map, p);
            if (count > visible) {
                visible = count;
                base = p;
            }
        }
    }

    Ray *rays = NULL;
    size_t ray_count = 0;
    size_t asteroid200 = 0;

    rays = malloc(rows * cols * sizeof(Ray));
    ray_count = find_ordered_rays(map, base, rays);
    asteroid200 = vaporise200(map, base, rays, ray_count);

    printf("[10/1] %li\n", visible);
    printf("[10/2] %li\n", asteroid200);

    free(rays);
    free(map);
}
