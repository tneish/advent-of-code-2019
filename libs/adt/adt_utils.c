#include <stdio.h>
#include <assert.h>
#include <stdbool.h>


#include "adt_utils.h"


static bool DEBUG = false;

/* Used for "if X in list<string>" */
int match_strings(void *l, void *r) 
{
    if (sdscmp((const sds)l, (const sds)r) == 0) {
        // Match
        return 1;
    } else {
        return 0;
    }
}

char *pp_list_of_strings(list *l) 
{
    sds s = sdsnew("{");
    listIter *li = listGetIterator(l,0);
    listNode *ln = NULL;
    while ((ln = listNext(li)) != NULL) {
        s = sdscatprintf(s, "%s ", (char *)listNodeValue(ln));
    }
    s = sdscat(s, "}");
    listReleaseIterator(li);
    return s;
}


/* Dictionary keys are node names as strings; each key value is a list of 
 * neighbour node names as strings.
 * start is name of node to start the search
 * goal is name of node to end the search
 * return value is distance - 2 (the puzzle does not count first and last 
 * edges).
 */
int breadth_first(dict* d, sds start, sds goal)
{
    dictEntry *de = 0;
    listIter *li_i = 0;
    listIter *li_j = 0;
    listNode *ln_i = 0;
    listNode *ln_j = 0;
    int distance = 0;  
    int retval = -1;
    int more_to_do = 1;

    if ((de = dictFind(d, start)) == NULL) {
        printf("Couldn't find start node %s!\n", start);
        assert(0);
    }

    list *nodes_to_visit = listCreate();
    list *visited_nodes = listCreate();
    listSetMatchMethod(visited_nodes, match_strings);

    if (listAddNodeTail(nodes_to_visit, start) == NULL) printf("Error adding start node to list!\n");
    if (listAddNodeTail(visited_nodes, start) == NULL) printf("Error adding start node to list!\n");
    if (DEBUG) printf("Breadth-first search starting at node %s\n", start);
    li_i = listGetIterator(nodes_to_visit, AL_START_HEAD);

    while (more_to_do) {
        if (DEBUG) {
            printf("Depth %d.\n", distance);
            sds pp_nodes_to_search = pp_list_of_strings(nodes_to_visit);
            sds pp_visited_nodes = pp_list_of_strings(visited_nodes);
            printf("List of nodes at this depth: %s.\n", pp_nodes_to_search);
            printf("List of visited nodes: %s.\n", pp_visited_nodes);
            sdsfree(pp_nodes_to_search);
            sdsfree(pp_visited_nodes);
            
        }
        more_to_do = 0;
        while ((ln_i = listNext(li_i)) != NULL) {
            if (DEBUG) printf("Visiting node %s.\n", (char *)listNodeValue(ln_i));
            if (sdscmp(goal, listNodeValue(ln_i)) == 0) {
                // Puzzle doesn't count first and last edge
                distance -= 2;
                // Found goal <goal> at distance <distance>
                //printf("Found goal %s at distance %d!\n", goal, distance);
                retval = distance;
                goto ret;
            }
            if ((de = dictFind(d, listNodeValue(ln_i))) == NULL) {
                printf("Could not find node %s in the dictionary!\n", (char *)listNodeValue(ln_i));
                goto node_visited;
            }
            // Each node as key in the dictionary holds a list of neighbors (strings) as value
            // For this node, add all its neighbors to the list of nodes to search at next depth
            li_j = listGetIterator(dictGetVal(de), AL_START_HEAD);
            while ((ln_j = listNext(li_j)) != NULL) {
                if (listSearchKey(visited_nodes, listNodeValue(ln_j)) == NULL) {
                    if (listAddNodeHead(visited_nodes, listNodeValue(ln_j)) == NULL) printf("Error adding to list!\n");
                    if (listAddNodeHead(nodes_to_visit, listNodeValue(ln_j)) == NULL) {
                        printf("Error adding to head of list!\n");
                    } else {
                        more_to_do = 1;
                    }
                } else {
                    if (DEBUG) printf("Already visited %s.\n", (char *)listNodeValue(ln_j));
                }
            }
            node_visited:
            listDelNode(nodes_to_visit, ln_i);

        }

        listRewind(nodes_to_visit, li_i);
        distance++;
    }

    ret:
    listReleaseIterator(li_i);
    listReleaseIterator(li_j);
    listRelease(nodes_to_visit);
    listRelease(visited_nodes);
    return retval;
}

