#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "utils.h"
#include "intcode.h"

static bool DEBUG = false;

#define NPHASES 5
int amp_series_output(const char* input_prog_str, int phase_settings[NPHASES])
{
    struct Intcode* ic = intcode_new();
    //intcode_set_debug(ic);
    int last = 0;
    for (int i = 0; i < NPHASES; i++) {
        (void)load_program(ic, input_prog_str);
        intcode_push_input(ic, last);
        intcode_push_input(ic, phase_settings[i]);        
        last = execute_program(ic);
    }
    intcode_free(ic);
    return last;
}
#undef NPHASES

#define NPHASES 5
int amp_series_output_with_feedback(const char* input_prog_str, int phase_settings[NPHASES])
{

    struct Intcode* ic[NPHASES];
    bool done = false;
    int last = 0;

    for (int i = 0; i < NPHASES; i++) {
        ic[i] = intcode_new();
        intcode_set_return_on_output(ic[i]);
        (void)load_program(ic[i], input_prog_str);
        intcode_push_input(ic[i], phase_settings[i]);
    }

    while (!done) {
        for (int i = 0; i < NPHASES; i++) {
            intcode_insert_input(ic[i], last);
            last = execute_program(ic[i]);
            if (intcode_is_done(ic[i])) done=true;
        }
    }

    for (int i = 0; i < NPHASES; i++) intcode_free(ic[i]);
    return last;

}
#undef NPHASES

/*
 * Find max output from amp series.
 * All permutations of phase settings 0-4 are tested.
 * Not clear in problem statement but no phase setting is allowed to repeat.
 */
#define NPHASES 5
int max_amp_series_output(const char* input_prog_str, int phase_settings[NPHASES], bool feedback)
{
    int max_output = 0;
    int phases_at_max_output = 0;
    int out;
    int i = 0;
    do {
        i++;
        out = feedback ? amp_series_output_with_feedback(input_prog_str, phase_settings) : amp_series_output(input_prog_str, phase_settings);
        if (out > max_output) {
            max_output = out;
            phases_at_max_output = phase_settings[0] * 10000 + \
                                   phase_settings[1] *  1000 + \
                                   phase_settings[2] *   100 + \
                                   phase_settings[3] *    10 + \
                                   phase_settings[4] *     1;
        }

    } while (next_lex_perm(phase_settings, NPHASES));
    if (DEBUG) printf("Max output %d with phase settings %05d; %d permutations.\n", max_output, phases_at_max_output, i);
    return max_output;
}
#undef NPHASES





int main(int argc, char* argv[])
{
    printf("--- Day 7, part 1 ---\n");
#   define NPHASES 5
    int in[NPHASES];
    size_t l = 0;

    char *day7_1_test1 = read_file_to_str("input/day7_1_test1.txt", &l);
    utils_array_init(in, NPHASES, 4, 3, 2, 1, 0);
    assert(amp_series_output(day7_1_test1, in) == 43210);
    free(day7_1_test1);

    char *day7_1_test2 = read_file_to_str("input/day7_1_test2.txt", &l);
    utils_array_init(in, NPHASES, 0, 1, 2, 3, 4);
    assert(amp_series_output(day7_1_test2, in) == 54321);
    free(day7_1_test2);

    char *day7_1_test3 = read_file_to_str("input/day7_1_test3.txt", &l);    
    utils_array_init(in, NPHASES, 1, 0 ,4, 3, 2);
    assert(amp_series_output(day7_1_test3, in) == 65210);
    free(day7_1_test3);

    char *day7 = read_file_to_str("input/day7.txt", &l);
    utils_array_init(in, NPHASES, 0, 1, 2, 3, 4);
    printf("Max output %d\n", max_amp_series_output(day7, in, false));  // 38500 
    free(day7);

    printf("--- Day 7, part 2 ---\n");

    char *day7_2_test1 = read_file_to_str("input/day7_2_test1.txt", &l);
    utils_array_init(in, NPHASES, 5, 6, 7, 8, 9);
    assert(max_amp_series_output(day7_2_test1, in, true) == 139629729);
    free(day7_2_test1);

    char *day7_2_test2 = read_file_to_str("input/day7_2_test2.txt", &l);
    utils_array_init(in, NPHASES, 5, 6, 7, 8, 9);
    assert(max_amp_series_output(day7_2_test2, in, true) == 18216);
    free(day7_2_test2);

    day7 = read_file_to_str("input/day7.txt", &l);
    utils_array_init(in, NPHASES, 5, 6, 7, 8, 9);
    printf("Max output %d\n", max_amp_series_output(day7, in, true));  // 33660560
    free(day7);
#   undef NPHASES

    return EXIT_SUCCESS;
}

