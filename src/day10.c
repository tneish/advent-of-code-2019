#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "utils.h"

static bool DEBUG = false;


/* Return true with d[0] width, d[1] height and map as char**[]
 * (array of pointers to rows as null-terminated strings. ) 
 */
bool load_map(const char *in, int d[2], char ***map)
{
    if (in == NULL) return false;
    const char delim[] = "\n";
    char *input = 0;
    char *end = 0;
    char *tk = 0;
    char **m = 0;

    input = strdup(in);
    end = input;

    // First row
    tk = strsep(&end, delim);
    d[0] = strlen(tk);  // Width
    m = calloc(1, sizeof(*m));
    m[0] = strdup(tk);
    
    d[1] = 1;
    while (tk != NULL) {
        tk = strsep(&end, delim);
        if ((tk != NULL) && (strlen(tk) != 0)) {
            d[1]++;
            char **mp = reallocarray(m, sizeof(*m), d[1]);
            assert(mp != NULL);
            mp[d[1] - 1] = strdup(tk);
            m = mp;
        }
    }
    *map = m;
    return true;
}

/* dim[0] width, dim[1] height. */
void pp_map(char **map, int dim[2])
{
    for (int i = 0; i < dim[1]; i++) {
        printf("%s\n", map[i]);
    }
    printf("\n");

}

/* dim[0] width, dim[1] height */
char ** mapcpy(char **src, int dim[2])
{
    char **m = 0;

    m = calloc(dim[1], sizeof(char*));
    assert(m != NULL);
    
    for (int i = 0; i < dim[1]; i++) {
        m[i] = strdup(src[i]);
        assert(m[i] != NULL);
    }

    return m;
}


bool off_map(int dim[2], int i, int j)
{
    return !((0 <= i) && (i <= (dim[0] - 1)) && \
            (0 <= j) && (j <= (dim[1] - 1)));
}


/* {x, y} is origin asteroid, {i, j} is astroid which casts shadow */
void draw_shadow(char **map, int dim[2], int x, int y, int i, int j)
{
    char s = 's';  // shadow

    // Smallest whole vector for shadow
    int gcd = utils_gcd(abs(i - x), abs(j - y));
    int ui = (i - x) / gcd;
    int uj = (j - y) / gcd;
    int ti = i + ui;
    int tj = j + uj;

    while (!off_map(dim, ti, tj)) {
        map[tj][ti] = s;
        ti += ui;
        tj += uj;
    }

}

int visit_horiz(char **map, int dim[2], int x, int y, int d)
{
    int v = 0;  // number of co-ords visited
    char a = '#';  // asteroid

    // Upper horizontal, distance d from x,y
    for (int i = x - d, j = y - d; i <= x + d; i++) {
        if (off_map(dim, i, j)) continue;
        if (memcmp(&(map[j][i]), &a, sizeof(a)) == 0) {
            draw_shadow(map, dim, x, y, i, j);
        }
        v++;
    }
    // Lower horizontal
    for (int i = x - d, j = y + d; i <= x + d; i++) {
        if (off_map(dim, i, j)) continue;
        if (memcmp(&(map[j][i]), &a, sizeof(a)) == 0) {
            draw_shadow(map, dim, x, y, i, j);
        }
        v++;
    }

    return v;
}

int visit_vert(char **map, int dim[2], int x, int y, int d)
{
    int v = 0;  // number of co-ords visited
    char a = '#';  // asteroid

    // Left vertical, distance d from x,y
    for (int i = x - d, j = y - d; j <= y + d; j++) {
        if (off_map(dim, i, j)) continue;
        if (memcmp(&(map[j][i]), &a, sizeof(a)) == 0) {
            draw_shadow(map, dim, x, y, i, j);
        }
        v++;
    }
    // Right vertical
    for (int i = x + d, j = y - d; j <= y + d; j++) {
        if (off_map(dim, i, j)) continue;
        if (memcmp(&(map[j][i]), &a, sizeof(a)) == 0) {
            draw_shadow(map, dim, x, y, i, j);
        }
        v++;
    }

    return v;
}

int num_asteroids(char **map, int dim[2])
{
    char a = '#';
    int n = 0;
    for (int l = 0; l < dim[1]; l++) {
        for (int m = 0; m < dim[0]; m++) {
            if (memcmp(&(map[l][m]), &a, sizeof(a)) == 0) {
                n += 1;
            }
        }
    }
    return n;
}

/* dim[0] width, dim[1] height.
 * {x,y} co-ordinates of asteroid 
 * return number of asteroids within line-of-sight.
 */
int get_nlos(char **map, int dim[2], int x, int y)
{

    int n = 0;
    int v = 0;     // number of visited co-ordinates
    int d = 1;

    while (v < (dim[0] * dim[1]) - 1) {
        // Horiz and vert lines of the square (perimeter) around {x, y} at distance d
        v += visit_horiz(map, dim, x, y, d);
        v += visit_vert(map, dim, x, y, d);
        d += 1;
    } // visit all co-ordinates on the map except {x,y}

    n = num_asteroids(map, dim);

    if (DEBUG) {
        printf("Map with shadows for station at {%d,%d} with %d astroids within LOS:\n", x, y, n - 1);
        pp_map(map, dim);
    }

    return n - 1;
}

int day10_1(const char* in)
{
    int d[2];
    char **map = 0;
    char a = '#';  // asteroid
    int nlos = 0;
    int nlos_max = 0;
    int nlos_max_ij[2] = {0, 0};

    if (!load_map(in, d, &map)) {
        printf("Could not load map!\n");
        return -1;
    }
    if (DEBUG) printf("Map width %d * height %d\n", d[0], d[1]);
    for (int i = 0; i < d[0]; i++) {
        for (int j = 0; j < d[1]; j++) {
            if (memcmp(&a, &(map[j][i]), sizeof(a)) != 0) continue;
            if (DEBUG) printf("Found an astroid!\n");
            char **m = mapcpy(map, d);
            if ((nlos = get_nlos(m, d, i, j)) > nlos_max) {
                nlos_max = nlos;
                utils_array_init(nlos_max_ij, 2, i, j);
            }
        } // y
    } // x

    printf("Station on asteroid at {%d,%d} would see most (%d) other asteroids within L.O.S\n", \
            nlos_max_ij[0], nlos_max_ij[1], nlos_max);

    return nlos_max;

}

int main (int argc, char *argv[]) 
{
    size_t l = 0;

    printf("--- Day 10, part 1 ---\n");
    char *day10_1_test1 = read_file_to_str("input/day10_1_test1.txt", &l);
    assert(day10_1(day10_1_test1) == 8);
    free(day10_1_test1);

    char *day10 = read_file_to_str("input/day10.txt", &l);
    assert(day10_1(day10) == 256);
    free(day10);

    return EXIT_SUCCESS;

}