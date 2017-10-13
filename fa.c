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
		//Allocation pour chaque état
	self->transitions = calloc(state_count, sizeof(state_set *));
	
	for (i = 0; i < self->alpha_count; i++){
		//Allocation pour chaque alpha de chaque état
		self->transitions[i] = calloc(alpha_count, sizeof(state_set));
		
		//Initialisation de chaque state_set des transitions
		int j;
		for (j = 0; j < alpha_count; j++){
		
			self->transitions[i][j].size = 0;
			self->transitions[i][j].capacity = 1;
			self->states = calloc(capacity, sizeof(size_t));
		
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

	self->states[state].is_initial = true;
}

//Ajout d'une transition
void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to){

	alphaletter = alpha-(int)a;

	self->transition[from][alphaletter].states[size] = to;
	self->transition[from][alphaletter].size++;
	
	if (self->size == self->capacity)
	{
		self->capacity *= 2;
		size_t *data = calloc(self->capacity, sizeof(size_t));
		memcpy(data, self->states, self->size * sizeof(size_t));
		free(self->states);
		self->states = data;
	}
}
