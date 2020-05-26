/**
 * Utility functions
 *
 */
#include <stdlib.h>

#define utils_array_init(P, N, ...)             \
_Generic ((P),                                  \
 int* : utils_int_array_init,                   \
 long* : utils_long_array_init,                 \
 default : utils_abort_array_init)(P, N, __VA_ARGS__)


void utils_int_array_init(int *p, const int n, ...);
void utils_long_array_init(long *p, const int n, ...);
void utils_abort_array_init(void *p, const int n, ...);

int get_digit(int number, int n);
int utils_gcd(int a, int b);

// Read to null-terminated string. NULL on error.
char *read_file_to_str(const char filename[], size_t *len);

int next_lex_perm(int *a, int n);