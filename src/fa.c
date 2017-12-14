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

void fa_destroy(struct fa *self)
{
  free(self->states);

	size_t i;
  for (i = 0; i < self->state_count; ++i)
  {
		size_t j;
    for (j = 0; j < self->alpha_count; ++j){
      free(self->transitions[i][j].states);
		}

    free(self->transitions[i]);
  }
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
		self->state_count--;

		size_t i;
		for (i=state; i < self->state_count-1; i++){
			self->states[i] = self->states[i+1];
			self->transitions[i] = self->transitions[i+1];
		}

		if (self->trash_state > state)
			self->trash_state--;


		for (i=0; i < self->state_count; i++){
			size_t j;
			for (j=0; j < self->alpha_count; j++){
				size_t k;
				for (k=0; k < self->transitions[i][j].size; k++){
					if (self->transitions[i][j].states[k] > state)
						self->transitions[i][j].states[k] = self->transitions[i][j].states[k]-1;
					else if (self->transitions[i][j].states[k] == state)
						fa_remove_transition(self, i, (char)(j+(int)'a'), k);
				}
			}
		}

		if (self->trash_state != -1 && self->trash_state > state)
			self->trash_state--;

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

	if (res)
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

	if (res)
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

			self->transitions[self->state_count - 1] = (struct state_set *) realloc(self->transitions[self->state_count - 1], self->alpha_count*sizeof(struct state_set));

			size_t i;
			for (i = 0; i < self->alpha_count; ++i)
			{
				self->transitions[self->state_count - 1][i].size = 0;
				self->transitions[self->state_count - 1][i].capacity = 1;
				self->transitions[self->state_count - 1][i].states = (size_t *) malloc(sizeof(size_t));
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

	fa_set_state_final(self, self->trash_state);
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

/*int* finished(int* ** tab, size_t size, const struct fa *nfa){

	size_t i;
	for (i = 0; i < size; i++){
		size_t j;
		for (j = 0; j < nfa->alpha_count; j++){
			size_t k;
			bool exists = false;
			for (k = 0; k < size; k++){
				if (tab[j+1][i] = tab[0][k]){
					exists = true;
				}
			}
			if (exists == false){
				return tab[j+1][i];
			}
		}
	}

	return NULL;
}

void fa_create_deterministic(struct fa *self, const struct fa *nfa){

	int* ** tableau = (int* **) calloc(nfa->alpha_count+1, sizeof(int* *));

	size_t rows = pow(2,nfa->state_count);

	size_t i;
	for (i = 0; i < nfa->alpha_count+1; i++){
		tableau[i] = (int* *) calloc(rows, sizeof(int*));
	}

	for (i = 0; i < rows; i++){
		tableau[i][0] = (int*) calloc(pow(2,nfa->state_count), sizeof(int));
		tableau[i][0] = NULL;
		tableau[i][1] = (int*) calloc(pow(2,nfa->state_count), sizeof(int));
		tableau[i][2] = (int*) calloc(pow(2,nfa->state_count), sizeof(int));
	}

	size_t tab_size = 1;
	size_t iter_trans = 0;
	tableau[0][0][0] = 0;


	int* new_state = (int*) calloc(pow(2,nfa->state_count), sizeof(int));

	for (i = 0; i < self->state_count; i++){
		if (nfa->states[i].is_final){
			new_state[iter_trans++] = nfa->transitions[0][0].states[i];
		}
	}

	size_t iter_etat = 0;
	size_t iter_alpha = 0;

	do {

		tableau[iter_alpha+1][iter_etat] = new_state;

		if (iter_alpha == nfa->alpha_count){
			iter_etat++;
			iter_alpha = 0;
		}
		else{
			iter_alpha++;
		}

		iter_trans = 0;

		for (i = 0; i < nfa->transitions[iter_etat][iter_alpha].size; i++){
				tableau[iter_alpha][iter_etat][iter_trans++] = nfa->transitions[iter_etat][iter_alpha].states[i];
		}

		new_state = finished(tableau, tab_size, nfa);

	}while (!(new_state == NULL && tableau[0][iter_etat+1] == NULL && iter_alpha == nfa->alpha_count));



}*/


void fa_remove_non_accessible_states(struct fa *self){

	printf("Removing non-accessible states\n");

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
			fa_remove_state(self, i);
			graph_create_from_fa(gr, self, false);

			for (i = 0; i < nb_initial_states; i++){
				graph_depth_first_search(gr, initial_states[i], visited_states);
			}
		}
	}
}

void fa_remove_non_co_accessible_states(struct fa *self){

	printf("Removing non-co-accessible states\n");

	struct graph *gr = (struct graph *) malloc(sizeof(struct graph));
	graph_create_from_fa(gr, self, false);

	size_t i, nb_final_states = 0;
	for (i = 0; i < self->state_count; i++)
	{
		if (self->states[i].is_final)
			nb_final_states++;
	}

	size_t iter_final = 0;
	size_t final_states[nb_final_states];

	for (i = 0; i < self->state_count; i++)
	{
		if (self->states[i].is_final){
			final_states[iter_final] = i;
			iter_final++;
		}
	}

	for (i = 0; i < self->state_count; i++){

		if (!self->states[i].is_final){

			bool path = false;
			size_t j;
			for (j = 0; j < nb_final_states; j++){
				if (graph_has_path(gr, i, final_states[j]))
					path = true;
			}

			if (!path){
				fa_remove_state(self, i);
				graph_create_from_fa(gr, self, false);

				iter_final = 0;
				size_t k;
				for (k = 0; k < self->state_count; k++)
				{
					if (self->states[k].is_final){
						final_states[iter_final] = k;
						iter_final++;
					}
				}

				i = 0;
			}
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
			graph_depth_first_search(self, self->state[state].next[i].nb, visited);
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
