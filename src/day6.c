#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "sds.h"
#include "dict.h"
#include "dict_types.h"
#include "utils.h"
#include "adt_utils.h"

static bool DEBUG = false;

int day6_1(const char input_filename[]) 
{
    char *tokeni = 0;
    char *tokenj = 0;
    dictEntry *de_path_start;
    dictEntry *de;
    dictIterator *di;
    size_t input_str_len;
    const char new_line[] = "\n";
    const char sep[] = ")";

    dict* orbits = dictCreate(&string_string_dict, NULL);
    char *input_str = read_file_to_str(input_filename, &input_str_len);
    char *end = input_str;

    tokeni = strsep(&end, new_line);
    while (tokeni != NULL) {
        tokenj = strsep(&tokeni, sep);
        if (strcmp(tokenj,"") == 0) goto next;
        sds orbitee = sdsnew(tokenj);
        tokenj = strsep(&tokeni, sep);
        if (strcmp(tokenj,"") == 0) goto next;
        sds orbiter = sdsnew(tokenj);
        
        if ((de = dictFind(orbits, orbiter)) == NULL) {
            if (dictAdd(orbits, orbiter, orbitee) != DICT_OK) {
                printf("Error adding entry %s to dictionary!\n", orbiter);
                continue;
            }
            if (DEBUG) printf("Adding entry k: %s v: %s\n", orbiter, orbitee);
        } else {
            printf("Double entry! %s\n", orbiter);
            assert(0);
        }
        next:
        tokeni = strsep(&end, new_line);
    }
    free(input_str);

    int total_orbits = 0;
    sds path_end = sdsnew("COM");
    di = dictGetIterator(orbits);
    while ((de_path_start = dictNext(di)) != NULL) {
        int orbits_back_to_com = 1;
        sds val = dictGetVal(de_path_start);
        if (DEBUG) printf("Path %s", (sds)dictGetKey(de_path_start));
        while (sdscmp(val, path_end) != 0) {
            if (DEBUG) printf("->%s", val);
            assert((de = dictFind(orbits, val)) != NULL);
            orbits_back_to_com += 1;
            val = dictGetVal(de);
        }
        if (DEBUG) printf("->COM. %d orbits.\n", orbits_back_to_com);
        total_orbits += orbits_back_to_com;
    }
    
    return total_orbits;
}

int day6_2(const char input_filename[]) 
{
    char *tokeni = 0;
    char *tokenj = 0;
    dictEntry *de;
    size_t input_str_len;
    const char new_line[] = "\n";
    const char sep[] = ")";

    // Key is planet name, value is pointer to a list of neighbour planet names
    dict* planets = dictCreate(&string_listptr_dict,NULL);
    char *input_str = read_file_to_str(input_filename, &input_str_len);
    char *end = input_str;

    tokeni = strsep(&end, new_line);
    while (tokeni != NULL) {
        tokenj = strsep(&tokeni, sep);
        if (tokenj == NULL) goto next;
        if (strcmp(tokenj,"") == 0) goto next;
        sds planet1 = sdsnew(tokenj);
        tokenj = strsep(&tokeni, sep);
        if (tokenj == NULL) goto next;
        if (strcmp(tokenj,"") == 0) goto next;
        sds planet2 = sdsnew(tokenj);
        
        // planet2 is a neighbour to planet1, and vice-versa
        sds p[2] = {planet1, planet2};
        for (int i = 0; i < 2; i++) {
            if ((de = dictFind(planets, p[i])) == NULL) {
                list *planet_neighbours = listCreate();
                if (listAddNodeTail(planet_neighbours, p[1-i]) == NULL) printf("Error creating list\n");
                if (dictAdd(planets, p[i], planet_neighbours) != DICT_OK) {
                    printf("Error adding key %s to dictionary!\n", p[i]);
                    continue;
                }
                sds pplist = pp_list_of_strings(planet_neighbours);
                if (DEBUG) printf("Adding entry k: %s v: %s\n", p[i], pplist);
                sdsfree(pplist);
            } else {
                list *planet_neighbours = dictGetVal(de);
                if (listAddNodeTail(planet_neighbours, p[1-i]) == NULL) printf("Error adding to list\n");
                sds pplist = pp_list_of_strings(planet_neighbours);
                if (DEBUG) printf("Updating entry k: %s, v: %s\n", p[i], pplist);
                sdsfree(pplist);
            }

        }

        next:
        tokeni = strsep(&end, new_line);
    }
    free(input_str);

    sds start = sdsnew("YOU");
    sds goal = sdsnew("SAN");
    // Since all edges are equal weight, simplest algorithm is a breadth-first search
    int distance = breadth_first(planets, start, goal);
    sdsfree(start);
    sdsfree(goal);
    return distance;
}

int main(int argc, char* argv[])
{
    printf("--- Day 6, part 1 ---\n");
    printf("%d total orbits.\n", day6_1("input/day6.txt"));  // 270768

    printf("--- Day 6, part 2 ---\n");
    assert(day6_2("input/day6_2_test.txt") == 4);
    printf("Found SAN at distance %d\n", day6_2("input/day6.txt"));  // 451

    return EXIT_SUCCESS;
}