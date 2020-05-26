#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#include "utils.h"
#include "intcode.h"


long long day9(const char* input_prog_str, int input_val) 
{
    struct Intcode* ic = 0;
    long long ret = 0;

    if ((ic = intcode_new()) == NULL) {
        printf("Could not create intcode computer!\n");
        ret = -1;
        goto ret_err;
    }
    //intcode_set_debug(ic);
    (void)load_program(ic, input_prog_str);
    intcode_push_input(ic, input_val);
    ret = execute_program(ic);
    intcode_free(ic);
    ret_err:
    return ret;
}


int main(int argc, char* argv[])
{
    size_t l = 0;

    printf("--- Day 9, part 1 ---\n");
    char *day9_1_test1 = read_file_to_str("input/day9_1_test1.txt", &l);
    assert(day9(day9_1_test1, 0) == 99);
    free(day9_1_test1);

    char *day9_1_test2 = read_file_to_str("input/day9_1_test2.txt", &l);
    assert(day9(day9_1_test2, 0) == 1219070632396864);
    free(day9_1_test2);

    char *day9_1_test3 = read_file_to_str("input/day9_1_test3.txt", &l);
    assert(day9(day9_1_test3, 0) == 1125899906842624);
    free(day9_1_test3);

    char *day9_in = read_file_to_str("input/day9.txt", &l);
    printf("%lld\n", day9(day9_in, 1));  // 3518157894

    printf("--- Day 9, part 2 ---\n");
    printf("%lld\n", day9(day9_in, 2));  // 80379
    free(day9_in);

    return EXIT_SUCCESS;

}