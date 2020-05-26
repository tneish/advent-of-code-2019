#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "intcode.h"

int day2_1(const char prog[]) 
{
    struct Intcode* ic = 0;
    int ret = 0;

    if ((ic = intcode_new()) == NULL) {
        printf("Could not create intcode computer!\n");
        return -1;
    }

    (void)load_program(ic, prog);
    (void)write_position(ic, 1, 12);
    (void)write_position(ic, 2, 2);
    (void)execute_program(ic);

    ret = read_position(ic, 0);
    printf("Position 0 after program halt: %d\n", ret);
    intcode_free(ic);
    return ret;
}

int day2_2(const char prog[], int target_output) 
{
    int in1;
    int in2;
    struct Intcode* ic = 0;

    if ((ic = intcode_new()) == NULL) {
        printf("Could not create intcode computer!\n");
        return -1;
    }

    for (in1 = 0; in1 <= 99; in1++) {
        for (in2 = 0; in2 <= 99; in2++) {
            (void)load_program(ic, prog);
            (void)write_position(ic, 1, in1);
            (void)write_position(ic, 2, in2);
            (void)execute_program(ic);
            if (read_position(ic, 0) == target_output) {
                printf("Found inputs: %d, %d!\n", in1, in2);
                goto end;
            }
        }
    }
    end:
    printf("100 * noun + verb = %d.\n", (100*in1) + in2);
    intcode_free(ic);
    return (100*in1) + in2;

}

int main (int argc, char *argv[]) 
{
    const char day2_intcode_program[] = "1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,1,13,19,2,9,19,23,1,23,6,27,1,13,27,31,1,31,10,35,1,9,35,39,1,39,9,43,2,6,43,47,1,47,5,51,2,10,51,55,1,6,55,59,2,13,59,63,2,13,63,67,1,6,67,71,1,71,5,75,2,75,6,79,1,5,79,83,1,83,6,87,2,10,87,91,1,9,91,95,1,6,95,99,1,99,6,103,2,103,9,107,2,107,10,111,1,5,111,115,1,115,6,119,2,6,119,123,1,10,123,127,1,127,5,131,1,131,2,135,1,135,5,0,99,2,0,14,0";
    
    printf("--- Day 2 ---\n");
    assert(day2_1(day2_intcode_program) == 7594646);
    assert(day2_2(day2_intcode_program, 19690720) == 3376);

    return EXIT_SUCCESS;

}