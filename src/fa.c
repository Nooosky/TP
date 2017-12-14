#include "../include/fa.h"

//Création et initialisation de l'automate
void fa_create(struct fa *self, size_t alpha_count, size_t state_count){

	self->alpha_count = alpha_count;//Taille de l'alphabet utilise
	self->state_count = state_count;//Nombre d'etats de l'automate

	self->states = (struct state *) calloc(self->state_count, sizeof(struct state));

	int i;
	for (i = 0; i < self->state_count; i++){

		self->states[i].is_initial = false;
		self->states[i].is_final = false;
	}

	//Allocation du tableau de transitions #PasFacile
		//Allocation pour chaque état
	self->transitions = (struct state_set **) calloc(state_count, sizeof(struct state_set *));

	for (i = 0; i < state_count; i++){
		//Allocation pour chaque alpha de chaque état
		self->transitions[i] = (struct state_set *) calloc(alpha_count, sizeof(struct state_set));

		//Initialisation de chaque state_set des transitions
		int j;
		for (j = 0; j < alpha_count; j++){

			self->transitions[i][j].size = 0;
			self->transitions[i][j].capacity = 1;
			self->transitions[i][j].states = (size_t *) calloc(self->transitions[i][j].capacity, sizeof(size_t));

		}
	}

	self->trash_state = -1;

}

void fa_destroy(struct fa *self){

	free(self->states);
	free(self->transitions);
	free(self);
}

//Ajout d'un été initial
void fa_set_state_initial(struct fa *self, size_t state){

	self->states[state].is_initial = true;

}

//Ajout d'un état final
void fa_set_state_final(struct fa *self, size_t state){

	self->states[state].is_final = true;
}


void fa_remove_state(struct fa *self, size_t state){

	if (self->state_count > state)
	{
		int i;
		for (i=state; i < self->state_count-1; i++){
			self->states[i] = self->states[i+1];
			self->transitions[i] = self->transitions[i+1];
		}

		self->state_count--;
		printf("State %zu removed\n", state);
	}
	else
		printf("State %zu doesn't exist\n", state);

}

//Ajout d'une transition
void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to){

	int alphaletter = ((int)alpha-(int)'a');

	self->transitions[from][alphaletter].states[self->transitions[from][alphaletter].size] = to;
	self->transitions[from][alphaletter].size++;

	if (self->transitions[from][alphaletter].size == self->transitions[from][alphaletter].capacity)
	{
		self->transitions[from][alphaletter].capacity *= 2;
		size_t *data = (size_t *) calloc(self->transitions[from][alphaletter].capacity, sizeof(size_t));
		memcpy(data, self->transitions[from][alphaletter].states, self->transitions[from][alphaletter].size * sizeof(size_t));

		free(self->transitions[from][alphaletter].states);
		self->transitions[from][alphaletter].states = data;
	}

	printf("%zu--%c-->%zu\n", from, alpha, to);

}

void fa_remove_transition(struct fa *self, size_t from, char alpha, size_t to){

	int alphaletter = ((int)alpha-(int)'a');

	int i;
	for (i = 0; i < self->transitions[from][alphaletter].size; i++){

		if (self->transitions[from][alphaletter].states[i] == to){

			int j;
			for (j=i; j < self->transitions[from][alphaletter].size-1; j++){
				self->transitions[from][alphaletter].states[j] = self->transitions[from][alphaletter].states[j+1];
			}

			self->transitions[from][alphaletter].size--;
		}
	}
}


size_t fa_count_transitions(const struct fa *self){

	size_t res = 0;

	int i;
	for (i=0; i < self->state_count; i++){
		int j;
		for (j=0; j < self->alpha_count; j++){
			res += self->transitions[i][j].size;
		}
	}

	return res;
}


bool fa_is_deterministic(const struct fa *self){

	bool res = true;

	int i;
	for (i=0; i < self->state_count; i++){
		int j;
		for (j=0; j < self->alpha_count; j++){
			if (self->transitions[i][j].size > 1){
				res = false;
				printf("Non-deterministic automaton\n");
				break;
			}
		}
	}

	printf("Deterministic automaton\n");
	return res;
}

bool fa_is_complete(const struct fa *self){

	bool res = true;

	int i;
	for (i=0; i < self->state_count; i++){
		int j;
		for (j=0; j < self->alpha_count; j++){
			if (self->transitions[i][j].size < 1){
				res = false;
				printf("Non-complete automaton\n");
				break;
			}
		}
	}

	printf("Complete automaton\n");
	return res;
}

void fa_make_complete(struct fa *self){

	if (!fa_is_complete(self)){

		printf("Automaton completion\n");

		if (self->trash_state == -1)
		{
			self->state_count++;
			self->states = (struct state *) realloc(self->states, self->state_count * sizeof(struct state));

			self->states[self->state_count - 1].is_initial = 0;
			self->states[self->state_count - 1].is_final = 0;

			size_t i;
			for (i = 0; i < self->alpha_count; ++i)
			{
				self->transitions[i] = (struct state_set *) realloc(self->transitions[i], self->state_count*sizeof(struct state_set));
				self->transitions[i][self->state_count - 1].size = 1;
				self->transitions[i][self->state_count - 1].states = (size_t *) malloc(sizeof(size_t));
				self->transitions[i][self->state_count - 1].states[0] = self->state_count - 1;
			}

			self->trash_state = self->state_count - 1;
		}

		size_t i;
		for (i = 0; i < self->state_count; ++i)
		{
			size_t j;
			for (j = 0; j < self->alpha_count; ++j)
			{
				if(self->transitions[i][j].size == 0)
				{
					fa_add_transition(self, i, (char)(j + 'a'), self->trash_state);
				}
			}
		}
	}

	fa_is_complete(self);
}

bool fa_is_language_empty(const struct fa *self){

	bool empty = true;

	struct graph *gr = (struct graph *) malloc(sizeof(struct graph));
	graph_create_from_fa(gr, self, false);

	size_t i, nb_initial_states = 0, nb_final_states = 0;
	for (i = 0; i < self->state_count; i++)
	{
		if (self->states[i].is_initial)
			nb_initial_states++;
		if (self->states[i].is_final)
			nb_final_states++;
	}

	size_t initial_states[nb_initial_states];
	size_t final_states[nb_final_states];

	size_t iter_init = 0, iter_final = 0;

	for (i = 0; i < self->state_count; i++)
	{
		if (self->states[i].is_initial){
			initial_states[iter_init] = i;
			iter_init++;
		}
		if (self->states[i].is_final){
			final_states[iter_final] = i;
			iter_final++;
		}
	}

	for (i = 0; i < nb_initial_states; i++)
	{
		size_t j;
		for (j = 0; j < nb_final_states; j++)
		{
			if (graph_has_path(gr, initial_states[i], final_states[j]))
				empty = false;
		}
	}

	return empty;
}


void fa_remove_non_accessible_states(struct fa *self){

	struct graph *gr = (struct graph *) malloc(sizeof(struct graph));
	graph_create_from_fa(gr, self, false);

	size_t i, nb_initial_states = 0;
	for (i = 0; i < self->state_count; i++)
	{
		if (self->states[i].is_initial)
			nb_initial_states++;
	}

	size_t iter_init = 0;
	size_t initial_states[nb_initial_states];

	for (i = 0; i < self->state_count; i++)
	{
		if (self->states[i].is_initial){
			initial_states[iter_init] = i;
			iter_init++;
		}
	}

	bool visited_states[self->state_count];

	for (i = 0; i < self->state_count; i++){
		visited_states[i] = false;
	}

	for (i = 0; i < nb_initial_states; i++){
		graph_depth_first_search(gr, initial_states[i], visited_states);
	}

	for (i = 0; i < self->state_count; i++){
		if (!visited_states[i]){
			fa_remove_state(self, (size_t)i);
		}
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


void fa_dot_print(const struct fa *self, FILE *out){

	size_t i;

	fprintf(out,"digraph finite_state_machine {\nrankdir=LR;\nsize=\"8,5\"\nnode [shape = doublecircle];\n");
	for (i = 0; i < self->state_count; i++) {
		if (self->states[i].is_final) {
			fprintf(out,"	%zu",i);
		}
	}

	fprintf(out,";\nnode [shape = circle];\n");
	for (i = 0; i < self->state_count; i++) {
		if (self->states[i].is_initial) {
			fprintf(out,"P%zu [shape = point, style=filed,color=white];\nP%zu -> %zu [ label = \"\" ];\n",i,i,i);
		}
	}

	//fprintf(out,"Transitions :\n");
	for (size_t current_state = 0; current_state < self->state_count; current_state++) {
		for (size_t current_letter = 0; current_letter < self->alpha_count; current_letter++) {
			for (size_t dest_state = 0; dest_state < self->transitions[current_state][current_letter].size; dest_state++) {
				fprintf(out,"%zu -> %zu [ label = \"%c\" ];\n",current_state ,self->transitions[current_state][current_letter].states[dest_state],((char)current_letter+'a'));
			}
		}
	}
	fprintf(out, "}");
}

void graph_create_from_fa(struct graph *self, const struct fa *automate, bool inverted){

	//Initialisation nombre d'états
	self->nb_states = automate->state_count;

	//Allocation du tableaux contenant les états
	self->state = (struct state_node *) calloc(self->nb_states, sizeof(struct state_node));

	size_t i;
	for(i = 0; i < self->nb_states; i++){
		self->state[i].nb = i;
	}


	for(i = 0; i < self->nb_states; i++){

		//Initialisation de nb_next, le nombre de transitions de chaque état
		size_t j;
		for(j = 0; j < automate->alpha_count; j++){
			self->state[i].nb_next += automate->transitions[i][j].size;
		}


		self->state[i].next = (struct state_node *) calloc(self->state[i].nb_next, sizeof(struct state_node));

		size_t trans = 0;

		for(j = 0; j < automate->alpha_count; j++){
			size_t k;
			for(k = 0; k < automate->transitions[i][j].size; k++){
				self->state[i].next[trans++] = self->state[automate->transitions[i][j].states[k]];
			}
		}
	}
}

void graph_depth_first_search(const struct graph *self, size_t state, bool *visited){

	visited[state] = true;

	size_t i;
	for (i = 0; i < self->state[state].nb_next; i++){
		if (visited[self->state[state].next[i].nb] == false){
			graph_depth_first_search(self, self->state[state].next->nb, visited);
		}
	}
}

bool graph_has_path(const struct graph *self, size_t from, size_t to){

	size_t nbstates = self->nb_states;
	bool states[nbstates];
	size_t i;
	for(i=0; i < nbstates; i++)
	 	states[i]= false;
	graph_depth_first_search(self, from, states);

	return (states[to] == true);
}

void graph_destroy(struct graph *self){

	size_t i;
	for (i = 0; i < self->nb_states; i++){
		free(self->state[i].next);
	}

	free(self->state);
	free(self);

}
