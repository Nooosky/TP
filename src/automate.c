#include <stdlib.h>
#include <stdio.h>
#include "../include/fa.h"


int main(int argc, char **argv){

	struct fa *automate = malloc(sizeof(struct fa));

	fa_create(automate,2,5);
	fa_set_state_initial(automate,0);
	fa_set_state_initial(automate,1);
	fa_set_state_final(automate,1);
	fa_set_state_final(automate,4);

	fa_add_transition(automate,0,'a',1);
	fa_add_transition(automate,0,'a',2);
	fa_add_transition(automate,0,'b',2);
	//fa_add_transition(automate,0,'a',3);
	fa_add_transition(automate,1,'a',3);
	fa_add_transition(automate,1,'b',3);
	fa_add_transition(automate,2,'a',3);
	fa_add_transition(automate,2,'b',4);
	fa_add_transition(automate,3,'a',3);
	fa_add_transition(automate,3,'b',4);
	fa_add_transition(automate,4,'a',4);
	fa_add_transition(automate,4,'b',4);
	fa_remove_transition(automate,0,'a',2);
	//fa_add_transition(automate,0,'a',2);

	fa_remove_state(automate, 3);

	fa_pretty_print(automate, stdout);

	printf("Transition number : %zu\n", fa_count_transitions(automate));

	if (fa_is_deterministic(automate))
		printf("Automate déterministe\n");
	if (fa_is_complete(automate))
		printf("Automate complet\n");

	FILE * graph;
	graph = fopen("../graph/automate.dot", "w");

	fa_dot_print(automate, graph);

	int ret = fclose(graph);
	if (ret == -1) {
		perror("fclose");
		exit(1);
	}


	return 0;
}
