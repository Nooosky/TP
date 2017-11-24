#include "fa.h"

//Création et initialisation de l'automate
void fa_create(struct fa *self, size_t alpha_count, size_t state_count){

	self->alpha_count = alpha_count;//Taille de l'alphabet utilise
	self->state_count = state_count;//Nombre d'etats de l'automate

	self->states = calloc(self->state_count, sizeof(struct state));

	int i;
	for (i = 0; i < self->state_count; i++){

		self->states[i].is_initial = false;
		self->states[i].is_final = false;
	}

	//Allocation du tableau de transitions #PasFacile
		//Allocation pour chaque état
	self->transitions = calloc(state_count, sizeof(struct state_set *));

	for (i = 0; i < state_count; i++){
		//Allocation pour chaque alpha de chaque état
		self->transitions[i] = calloc(alpha_count, sizeof(struct state_set));

		//Initialisation de chaque state_set des transitions
		int j;
		for (j = 0; j < alpha_count; j++){

			self->transitions[i][j].size = 0;
			self->transitions[i][j].capacity = 1;
			self->transitions[i][j].states = calloc(self->transitions[i][j].capacity, sizeof(size_t));

		}
	}
}

void fa_destroy(struct fa *self){

	free(self->states);
	free(self->transitions);
	free(self);
}

//Ajou d'un été initial
void fa_set_state_initial(struct fa *self, size_t state){

	self->states[state].is_initial = true;

}

//Ajout d'un état final
void fa_set_state_final(struct fa *self, size_t state){

	self->states[state].is_final = true;
}

//Ajout d'une transition
void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to){

	int alphaletter = ((int)alpha-(int)'a');

	self->transitions[from][alphaletter].states[self->transitions[from][alphaletter].size] = to;
	self->transitions[from][alphaletter].size++;

	if (self->transitions[from][alphaletter].size == self->transitions[from][alphaletter].capacity)
	{
		self->transitions[from][alphaletter].capacity *= 2;
		size_t *data = calloc(self->transitions[from][alphaletter].capacity, sizeof(size_t));
		memcpy(data, self->transitions[from][alphaletter].states, self->transitions[from][alphaletter].size * sizeof(size_t));
<<<<<<< HEAD
=======

		printf("From %zu with %c to %zu = %zu\n", from, alpha, to, self->transitions[from][alphaletter].states[self->transitions[from][alphaletter].size-1]);
		printf("From %zu with %c to %zu = %zu\n", from, alpha, to, data[self->transitions[from][alphaletter].size-1]);

>>>>>>> 02d42c3b46a589f2b7df1b18b8ae7ea832b1f20e
		free(self->transitions[from][alphaletter].states);
		self->transitions[from][alphaletter].states = data;
	}

}

//Affichage simple d'un automate
void fa_pretty_print(const struct fa *self, FILE *out){

	size_t i;

	fprintf(out,"Intial states :\n");
	for (i = 0; i < self->state_count; i++) {
		if (self->states[i].is_initial) {
			fprintf(out,"	%zu",i);
		}
	}

	fprintf(out,"\nFinal states :\n");
	for (i = 0; i < self->state_count; i++) {
		if (self->states[i].is_final) {
			fprintf(out,"	%zu",i);
		}
	}

	fprintf(out,"\nTransitions :\n");
	for (size_t current_state = 0; current_state < self->state_count; current_state++) {
		fprintf(out,"	For state %zu :\n",current_state);
		for (size_t current_letter = 0; current_letter < self->alpha_count; current_letter++) {
			fprintf(out,"		For letter %c :",(char)(current_letter+(int)'a'));
			for (size_t dest_state = 0; dest_state < self->transitions[current_state][current_letter].size; dest_state++) {
				fprintf(out,"	%zu",self->transitions[current_state][current_letter].states[dest_state]);
			}
			fprintf(out, "\n");
		}
	}

}
