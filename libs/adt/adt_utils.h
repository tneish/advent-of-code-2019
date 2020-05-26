#include "dict.h"
#include "adlist.h"
#include "sds.h"


/* Pretty-print list of strings. Caller is responsible for freeing memory after use. */
char *pp_list_of_strings(list *l);

/* Dictionary keys are node names as strings; each key value is a list of 
 * neighbour node names as strings.
 * start is name of node to start the search
 * goal is name of node to end the search
 * return value is distance - 2 (the puzzle does not count first and last 
 * edges).
 */
int breadth_first(dict* d, sds start, sds goal);