#include "fa.h"

//Création et initialisation de l'automate
void fa_create(struct fa *self, size_t alpha_count, size_t state_count){

	self->alpha_count = alpha_count;
	self->state_count = state_count;
	
	self->states = calloc(self->state_count, sizeof(struct state));
	
	int i;
	for (i = 0; i < self->state_count; i++){
	
		self->state[i].is_initial = false;
		self->state[i].is_final = false;
	}
	
	//Allocation du tableau de transitions #PasFacile
	self->transitions = calloc(state_count, sizeof(state_set *));
	
	for (i = 0; i < self->alpha_count; i++){
	
		self->transitions[i] = calloc(alpha_count, sizeof(state_set));
		
		int j;
		for (j = 0; j < alpha_count; j++){
		
			self->transitions[i][j].size = 0;
			self->transitions[i][j].capacity = 1;
			self->states = calloc(capacity, sizeof(size_t));
		
		}
		
	}
	
	

}

void fa_destroy(struct fa *self);

void fa_set_state_initial(struct fa *self, size_t state);
void fa_set_state_final(struct fa *self, size_t state);
