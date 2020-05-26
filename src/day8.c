#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void day8()
{
    // 15000 chars
    const char in_file[] = "input/day8.txt";
#   define LAYERS 100
#   define DIGITS 10
    int d[LAYERS][DIGITS];
    const int layersize = 25 * 6;
    size_t l = 0;

    memset(&d, 0, LAYERS*DIGITS*sizeof(int));

    char *in = read_file_to_str(in_file, &l);

    for (int i = 0; i < strlen(in); i++) {
        char c[] = {in[i], '\0'};
        d[i/layersize][atoi((const char*)c)]++;
    }

    int min_layer = -1;
    unsigned int nzeroes = 0;
    nzeroes--;
    for (int j = 0; j < LAYERS; j++) {
        if (d[j][0] < nzeroes) {
            nzeroes = d[j][0];
            min_layer = j;
        }
    }
    int r = d[min_layer][1] * d[min_layer][2];
    printf("the number of 1 digits multiplied by the number of 2 digits is %d.\n", r);
    //assert(r == 1806);

    char c;
    for (int k = 0; k < layersize; k++) {
        for (int l = 0; l < LAYERS; l++) {
            c = *(in + (l*layersize) + k);
            if (c != '2') break;
        } // layers
        if (k % 25 == 0) printf("\n");
        printf("%c", c == '0' ? '#' : ' ');
    } // pixels
    printf("\n");

    // "JAFRA"

    free(in);
#   undef LAYERS
#   undef DIGITS
}

int main(int argc, char* argv[])
{
    printf("--- Day 8 ---\n");
    day8();

    return EXIT_SUCCESS;
}

