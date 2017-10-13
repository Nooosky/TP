#ifndef FA_H
#define FA_H

#include <stdbool.h>

struct state {

	bool is_initial;
	bool is_final;
};


struct fa {

	size_t alpha_count;
	size_t state_count;

	struct state *states;
	
	struct state_set **transitions;
};

struct state_set {

	size_t size;
	size_t capacity;
	size_t *states;
}

void fa_create(struct fa *self, size_t alpha_count, size_t state_count);

void fa_destroy(struct fa *self);

void fa_set_state_initial(struct fa *self, size_t state);
void fa_set_state_final(struct fa *self, size_t state);

void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to);


#endif //FA_H
