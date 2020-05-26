#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

#include "utils.h"


static int _pow10(int n) 
{
    static int pow10[10] = {
        1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
    };
    assert(n<10);
    return pow10[n];
}

int get_digit(int number, int n) 
{
    // n starting at 0
    return (number % _pow10(n+1)) / _pow10(n);
}

// Read to null-terminated string. NULL on error.
char *read_file_to_str(const char filename[], size_t *len) 
{
    char *buf = 0;
    long _len;
    
    FILE *pf = fopen(filename, "rb");

    if (!pf) goto error;

    fseek(pf, 0, SEEK_END);
    _len = ftell(pf);
    fseek(pf, 0, SEEK_SET);

    buf = malloc(_len + 1);
    if (!buf) goto error;

    fread(buf, 1, _len, pf);
    buf[_len] = '\0';

    if (!len) goto error;
    // len is same as returned by strlen (i.e. without counting null-byte)
    *len = _len;
    fclose (pf);
    return buf;
    
    error:
    if (pf) fclose(pf);
    if (buf) free(buf);
    return NULL;
}


/* 
 * next lexicographical permutation.
 * Used from https://rosettacode.org/wiki/Permutations#C.
 * See LICENSE file.
 */
int next_lex_perm(int *a, int n) {
#	define swap(i, j) {t = a[i]; a[i] = a[j]; a[j] = t;}
	int k, l, t;
 
	/* 1. Find the largest index k such that a[k] < a[k + 1]. If no such
	      index exists, the permutation is the last permutation. */
	for (k = n - 1; k && a[k - 1] >= a[k]; k--);
	if (!k--) return 0;
 
	/* 2. Find the largest index l such that a[k] < a[l]. Since k + 1 is
	   such an index, l is well defined */
	for (l = n - 1; a[l] <= a[k]; l--);
 
	/* 3. Swap a[k] with a[l] */
	swap(k, l);
 
	/* 4. Reverse the sequence from a[k + 1] to the end */
	for (k++, l = n - 1; l > k; l--, k++)
		swap(k, l);
	return 1;
#	undef swap
}

/* Greatest common denominator. Euclidean algorithm */
int utils_gcd(int a, int b) 
{
	int r;
	while (b > 0) 
	{
		r = a % b;
		a = b;
		b = r;
	}

	return a;
}

/*
 * Array initialization functions.
 */
void utils_int_array_init(int *p, const int n, ...) 
{
    va_list args;
    va_start(args, n);
    for(int i = 0; i < n; ++i) {
        p[i] = va_arg(args, int);
    }
    va_end(args);
}

void utils_lont_array_init(long *p, const int n, ...) 
{
    va_list args;
    va_start(args, n);
    for(int i = 0; i < n; ++i) {
        p[i] = va_arg(args, long);
    }
    va_end(args);
}

void utils_abort_array_init(void *p, const int n, ...) 
{
    assert(0);
}


