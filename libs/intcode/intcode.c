/**
 * Intcode programmer
 *
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include "intcode.h"
#include "utils.h"



struct Intcode {
    bool DEBUG;
    bool return_on_out;
    bool done;

    long long last_output;

    int *_input;
    int num_inputs;

    int pc;
    long long * _code_array;
    int _code_array_len;
    int relbase;
};

struct Intcode* intcode_new() 
{
    struct Intcode* p;
    if ((p = calloc(1, sizeof(*p))) == NULL) return NULL;

    p->_code_array_len = 0;
    p->DEBUG = false;
    p->return_on_out = false;
    p->done = false;
    p->pc = 0;
    p->last_output = -1;
    p->_input = 0;
    p->num_inputs = 0;
    p->relbase = 0;

    return p;
}

void intcode_free(struct Intcode* this)
{
    if (this == NULL) return;
    if (this->_code_array) free(this->_code_array);
    free(this);
}

void intcode_set_debug(struct Intcode* this) 
{
    this->DEBUG = true;
}

void intcode_clear_debug(struct Intcode* this) 
{
    this->DEBUG = false;
}

void intcode_set_return_on_output(struct Intcode* this)
{
    this->return_on_out = true;
}

void intcode_clear_return_on_output(struct Intcode* this)
{
    this->return_on_out = false;
}

long long intcode_get_last_output(struct Intcode* this)
{
    return this->last_output;
}

bool intcode_is_done(struct Intcode* this)
{
    return this->done;
}

void intcode_push_input(struct Intcode* this, int val)
{
    if (this->_input != 0) {
        int *np = reallocarray(this->_input, sizeof(int), this->num_inputs + 1);
        assert(np != NULL);
        this->_input = np;
        this->num_inputs += 1;
    } else {
        this->_input = calloc(sizeof(int), 1);
        assert(this->_input != NULL);
        this->num_inputs += 1;
    }

    this->_input[this->num_inputs - 1] = val;
}

void intcode_insert_input(struct Intcode* this, int val)
{
    if (this->_input == 0) {
        intcode_push_input(this, val);
        return;
    } 
    int *np = calloc(sizeof(int), this->num_inputs + 1);
    assert(np != NULL);
    np[0] = val;
    for (int i = 0; i < this->num_inputs; i++) {
        np[i+1] = this->_input[i];
    }
    free(this->_input);
    this->_input = np;
    this->num_inputs += 1;

}

static int pop_input(struct Intcode* this)
{
    assert(this->num_inputs > 0);
    int ret = this->_input[this->num_inputs - 1];

    if (this->num_inputs == 1) {
        free(this->_input);
        this->_input = 0;
        this->num_inputs = 0;
    } else {
        int *np = reallocarray(this->_input, sizeof(this->_input), this->num_inputs - 1);
        assert(np != NULL);
        this->_input = np;
        this->num_inputs -= 1;
    }
    
    return ret;
}

void print_code_array(struct Intcode* this) 
{
    for (int i = 0; i < this->_code_array_len; i++) 
        printf("position %d: %lld\n", i, *(this->_code_array + i));
}

static void reset_program(struct Intcode* this)
{
    this->pc = 0;
    this->done = false;
    this->last_output = -1;
    this->relbase = 0;
    for (int i = 0; i < this->num_inputs; i++) (void)pop_input(this);

}

/* Overwrites any existing program, and additionally resets Intcode computer */
int load_program(struct Intcode* this, const char *prog) 
{
    const char delim[] = ",";
    char *r;
    char *end;
    char *tk;
    int i;

    r = strdup(prog);
    end = r;

    /* Find how many tokens in the input */
    i = 0;
    tk = strsep(&end, delim);
    while (tk != NULL) {
        tk = strsep(&end, delim);
        i++;
    }
    this->_code_array_len = i;

    // Day 9, "available memory should be much larger than the initial program."
    this->_code_array_len = (this->_code_array_len * 2 > 10000) ? this->_code_array_len * 2 : 10000;

    if (this->_code_array != 0) {
        if (this->DEBUG) printf("Previous program found. Resetting program storage.\n");
        long long *np = reallocarray(this->_code_array, sizeof(*(this->_code_array)), this->_code_array_len);
        assert(np != NULL);
        memset(np, 0, this->_code_array_len);
        this->_code_array = np;
    } else {
        this->_code_array = calloc(sizeof(*(this->_code_array)), this->_code_array_len);
    }
    
    if (this->DEBUG) printf("Input has %d codes, padded to %d.\n", i, this->_code_array_len);
    free(r);
    
    /* Write tokens into array */
    i = 0;
    r = strdup(prog);
    end = r;
    tk = strsep(&end, delim);
    while (tk != NULL) { 
        *(this->_code_array + i) = atoll(tk);
        i++;
        tk = strsep(&end, delim);
    }
    if (this->DEBUG) {
        //print_code_array(this);
    }
    reset_program(this);
    free(r);
    return 0;
}

void write_position(struct Intcode* this, long long position, long long newcode) 
{
    // Position starts from 0
    assert(position < this->_code_array_len);
    *(this->_code_array + position) = newcode;

}

long long read_position(struct Intcode* this, long long position) 
{
    assert(position < this->_code_array_len);
    return *(this->_code_array + position);
}

static long long get_dest(struct Intcode* this, long long pos, int mode)
{
    switch (mode) {
        case 0:
        // Positional mode  
        return read_position(this, pos);
        case 1:
        // Immediate mode
        return pos;
        case 2:
        // Relative mode
        return read_position(this, pos) + this->relbase;
        default:
        printf("Error! Unknown parameter mode.\n");
        assert(0);

    }
}

static long long get_param(struct Intcode* this, long long pos, int mode)
{
    return read_position(this, get_dest(this, pos, mode));
}

long long execute_program(struct Intcode* this) 
{
    // Holds position of current opcode
    int instruction;
    int opcode;
    long long param3;
    // Parameter modes, 0 == position mode, 1 == immediate mode, 2 == position+relative base
    int param1_mode;
    int param2_mode;
    int param3_mode;
    long long input_val1;
    long long input_val2;
    long long out;  

    while (1) {
        if (this->pc > this->_code_array_len - 1) {
            // Reached end of program before 99 opcode.
            if (this->DEBUG) printf("PC passed end of program input. Stopping.\n");
            this->done = true;
            break;
        }
        instruction = *(this->_code_array + this->pc);
        param1_mode = get_digit(instruction, 2);
        param2_mode = get_digit(instruction, 3);
        param3_mode = get_digit(instruction, 4);
        opcode = instruction % 100;
        if (this->DEBUG) printf("PC: %d, instruction: %d, opcode: %d, pmodes: {%d,%d,%d}.\n", 
                this->pc, instruction, opcode, param1_mode, param2_mode, param3_mode);

        // Decode and run opcode
        if (opcode == 99) {
            // Program normal termination
            if (this->DEBUG) printf("Processed opcode 99 at PC %d. Stopping.\n", this->pc);
            this->done = true;
            return this->last_output;

        } else if (opcode == 1) {
            // Addition
            input_val1 = get_param(this, this->pc+1, param1_mode);
            input_val2 = get_param(this,  this->pc+2, param2_mode);
            param3 = get_dest(this, this->pc+3, param3_mode);

            // "Parameters that an instruction writes to will never be in immediate mode."
            assert(param3_mode != 1);
            out = input_val1 + input_val2;
            if (this->DEBUG) printf("Processing opcode addition: {%lld + %lld = %lld} -> pos %lld\n",
                 input_val1, input_val2, out, param3);
            (void)write_position(this, param3, out);
            this->pc += 4;

        } else if (opcode == 2) {
            // Multiplication
            input_val1 = get_param(this, this->pc+1, param1_mode);
            input_val2 = get_param(this, this->pc+2, param2_mode);
            param3 = get_dest(this, this->pc+3, param3_mode);

            // "Parameters that an instruction writes to will never be in immediate mode."
            assert(param3_mode != 1);
            out = input_val1 * input_val2;
            if (this->DEBUG) printf("Processing opcode multip  : {%lld * %lld = %lld} -> pos %lld\n",
                 input_val1, input_val2, out, param3);
            (void)write_position(this, param3, out);
            this->pc += 4;

        } else if (opcode == 3) {
            // Store program input into position
            // "Parameters that an instruction writes to will never be in immediate mode."
            assert(param1_mode != 1);            
            input_val1 = get_dest(this, this->pc+1, param1_mode);
            //param1 = read_position(this, this->pc+1);
            int inval = pop_input(this);
            if (this->DEBUG) printf("Processing opcode store input : %d -> pos %lld\n",
                 inval, input_val1);


            (void)write_position(this, input_val1, inval);
            this->pc += 2;

        } else if (opcode == 4) {
            // Output value at position
            out = get_param(this, this->pc+1, param1_mode);
            //printf("Output: %lld\n", out);
            this->last_output = out;
            this->pc += 2;
            if (this->return_on_out == true) return out;
            
        } else if (opcode == 5) {
            // jump-if-true
            input_val1 = get_param(this, this->pc+1, param1_mode);
            input_val2 = get_param(this, this->pc+2, param2_mode);
            if (input_val1 != 0) {
                this->pc = input_val2;
            } else {
                this->pc += 3;
            }

        } else if (opcode == 6) {
            // jump-if-false
            input_val1 = get_param(this, this->pc+1, param1_mode);
            input_val2 = get_param(this, this->pc+2, param2_mode);   
            if (input_val1 == 0) {
                this->pc = input_val2;
            } else {
                this->pc += 3;
            }

        } else if (opcode == 7) {
            // less than
            input_val1 = get_param(this, this->pc+1, param1_mode);
            input_val2 = get_param(this, this->pc+2, param2_mode);
            param3 = get_dest(this, this->pc+3, param3_mode);
            // "Parameters that an instruction writes to will never be in immediate mode."
            assert(param3_mode != 1);
            input_val1 < input_val2 ? write_position(this, param3, 1) : write_position(this, param3, 0);
            this->pc += 4;

        } else if (opcode == 8) {
            // equals
            input_val1 = get_param(this, this->pc+1, param1_mode);
            input_val2 = get_param(this, this->pc+2, param2_mode);
            param3 = get_dest(this, this->pc+3, param3_mode);
            // "Parameters that an instruction writes to will never be in immediate mode."
            assert(param3_mode != 1);
            input_val1 == input_val2 ? write_position(this, param3, 1) : write_position(this, param3, 0);
            this->pc += 4;
        
        } else if (opcode == 9) {
            // addition to relative base
            input_val1 = get_param(this, this->pc+1, param1_mode);
            int newbase = this->relbase + input_val1;
            if (this->DEBUG) printf("Processing opcode adj.relbase: {base %d + %lld = %d}\n",
            this->relbase, input_val1, newbase);
            this->relbase = newbase;
            this->pc += 2;

        }
    }
    if (this->DEBUG) {
        print_code_array(this);
    }
    return 0;

}