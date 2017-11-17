#include <stdlib.h>
#include <stdio.h>
#include "fa.h"


int main(int argc, char **argv){

	struct fa *automate = malloc(sizeof(struct fa));

	fa_create(automate,2,5);
	fa_set_state_initial(automate,0);
	fa_set_state_initial(automate,1);
	fa_set_state_final(automate,1);
	fa_set_state_final(automate,4);

	fa_add_transition(automate,0,'a',1);
	fa_add_transition(automate,0,'a',2);
	fa_add_transition(automate,0,'a',3);
	fa_add_transition(automate,1,'b',3);
	fa_add_transition(automate,2,'a',3);
	fa_add_transition(automate,2,'b',4);
	fa_add_transition(automate,3,'a',3);
	fa_add_transition(automate,3,'b',4);
	fa_add_transition(automate,4,'b',4);

	fa_pretty_print(automate, stdout);



	return 0;
}
