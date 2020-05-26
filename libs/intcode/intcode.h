/**
 * Intcode programmer
 *
 */
#include <stdbool.h>

struct Intcode;

struct Intcode* intcode_new();

bool intcode_is_initialized(struct Intcode* this);
bool intcode_is_done(struct Intcode* this);
void intcode_set_debug(struct Intcode* this);
void intcode_clear_debug(struct Intcode* this);
void intcode_set_return_on_output(struct Intcode* this);
void intcode_clear_return_on_output(struct Intcode* this);
long long intcode_get_last_output(struct Intcode* this);
void intcode_push_input(struct Intcode* this, int val);
void intcode_insert_input(struct Intcode* this, int val);


int load_program(struct Intcode* this, const char* program);
void write_position(struct Intcode* this, long long position, long long newcode);
long long read_position(struct Intcode* this, long long position);

long long execute_program(struct Intcode* this);

void intcode_free(struct Intcode* this);