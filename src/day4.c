#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "utils.h"

static int DEBUG = 0;

void day4(int start, int end) 
{
    int n_passwords_day41 = 0;
    int n_passwords_day42 = 0;
    int d[6];

    assert(start <= 999999);
    assert(end <= 999999);

    for (int i = start; i <= end; i++) {
        // Each iteration is a new candidate password to be evaluated
        int n_conseq = 1;
        int day41_candidate = 0;
        int day42_candidate = 0;

        for (int j = 0; j < 6; j++) {
            d[j] = get_digit(i, 5 - j);

            // First number always OK
            if (j == 0) continue;

            if (d[j] < d[j-1]) {
                // Failed
                goto next_candidate;
            }

            if (d[j] == d[j-1]) {
                n_conseq++;
                if (j < 5) continue;
            }
            if (n_conseq >= 2) day41_candidate = 1;
            if (n_conseq == 2) day42_candidate = 1;
            // Reset
            n_conseq = 1;
        }

        if (day41_candidate == 1) {
            if (DEBUG) printf("Candidate password 4_1: %d.\n", i);
            n_passwords_day41++;
        }
        if (day42_candidate == 1) {
            if (DEBUG) printf("Candidate password 4_2: %d.\n", i);
            n_passwords_day42++;
        }
        next_candidate:
        continue;
    }

    printf("Day4_1: %d candidate passwords between %d and %d.\n", n_passwords_day41, start, end);
    printf("Day4_2: %d candidate passwords between %d and %d.\n", n_passwords_day42, start, end);
    
}

int main(int argc, char* argv[])
{
    day4(236491, 713787);
    return EXIT_SUCCESS;
}