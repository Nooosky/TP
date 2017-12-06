#ifndef FA_H
#define FA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

struct state {

	bool is_initial;
	bool is_final;
};


struct fa {

	size_t alpha_count; //Taille de l'alphabet
	size_t state_count; //Nombre d'etats de l'automate

	struct state *states; //Ensemble d'etats

	struct state_set **transitions; //Ensemble de transitions
};

struct state_set {

	size_t size;
	size_t capacity;
	size_t *states;
};

void fa_create(struct fa *self, size_t alpha_count, size_t state_count);

void fa_destroy(struct fa *self);

void fa_set_state_initial(struct fa *self, size_t state);
void fa_set_state_final(struct fa *self, size_t state);

void fa_remove_state(struct fa *self, size_t state);

void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to);
void fa_remove_transition(struct fa *self, size_t from, char alpha, size_t to);

size_t fa_count_transitions(const struct fa *self);

void fa_pretty_print(const struct fa *self, FILE *out);

<<<<<<< HEAD
bool fa_is_deterministic(const struct fa *self);
bool fa_is_complete(const struct fa *self);
void fa_make_complete(struct fa *self);

=======
void fa_dot_print(const struct fa *self, FILE *out);
>>>>>>> 1e92dc775a445f7ec35929893a027782073bf364


#endif //FA_H
