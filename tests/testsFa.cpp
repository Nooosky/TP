#include "../src/fa.c"
#include <gtest/gtest.h>

/*virtual void SetUp(){
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
}

virtual void TearDown(struct fa *automate){
    free(automate);
}*/

TEST(AutomateTest, TestCreationAutomate) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  ASSERT_NO_THROW(fa_create(automate,1,1));
  free(automate);
}

TEST(AutomateTest, TestDestrucAutomate) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  ASSERT_NO_THROW(fa_create(automate,1,1));
  ASSERT_NO_THROW(fa_destroy(automate));
}

TEST(AutomateTest, TestCreationEtatInitial) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  ASSERT_NO_THROW(fa_create(automate,1,1));
  ASSERT_NO_THROW(fa_set_state_initial(automate,0););
  ASSERT_TRUE(automate->states[0].is_initial);
  fa_destroy(automate);
}

TEST(AutomateTest, TestCreationEtatFinal) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  ASSERT_NO_THROW(fa_create(automate,1,1));
  ASSERT_NO_THROW(fa_set_state_final(automate,0););
  ASSERT_TRUE(automate->states[0].is_final);
  fa_destroy(automate);
}

TEST(AutomateTest, TestCreationTransition) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  ASSERT_NO_THROW(fa_create(automate,1,2));
  ASSERT_NO_THROW(fa_add_transition(automate,0,'a',1));
  ASSERT_EQ(automate->transitions[0][0].states[0],1);
  fa_destroy(automate);
}

TEST(AutomateTest, TestPrettyPrint) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
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
	fa_add_transition(automate,4,'a',4);

  fa_pretty_print(automate, stdout);

  fa_destroy(automate);
}

TEST(AutomateTest, TestSupprTransition_1) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,1,2);
  fa_add_transition(automate,0,'a',1);
  size_t i = automate->transitions[0][0].size;
  fa_remove_transition(automate,0,'a',1);
  ASSERT_LT(automate->transitions[0][0].size,i);
  fa_destroy(automate);
}

TEST(AutomateTest, TestSupprTransition_2) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,1,2);
  size_t i = automate->transitions[0][0].size;
  fa_remove_transition(automate,0,'a',1)  ;
  ASSERT_EQ(automate->transitions[0][0].size,i);
  fa_destroy(automate);
}

 //Pas fini il faut encore chercher si on supprime bien le bon etat

TEST(AutomateTest, TestSupprEtat) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,1,2);
  size_t i = automate->state_count;
  fa_remove_state(automate,1);
  ASSERT_LT(automate->state_count,i);
  fa_destroy(automate);
}

TEST(AutomateTest, TestCompteurTransition_1) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,0,'a',2);
  fa_add_transition(automate,0,'b',2);
  ASSERT_EQ(3,fa_count_transitions(automate));
  fa_destroy(automate);
}

TEST(AutomateTest, TestCompteurTransition_2) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
  ASSERT_EQ(0,fa_count_transitions(automate));
  fa_destroy(automate);
}

TEST(AutomateTest, TestEstDeterimiste_1) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,0,'a',2);
  ASSERT_FALSE(fa_is_deterministic(automate));
  fa_destroy(automate);
}

TEST(AutomateTest, TestEstDeterimiste_2) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,0,'b',2);
  ASSERT_TRUE(fa_is_deterministic(automate));
  fa_destroy(automate);
}

TEST(AutomateTest, TestEstComplet_1) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,0,'b',2);
  fa_add_transition(automate,1,'a',1);
  fa_add_transition(automate,1,'b',0);
  fa_add_transition(automate,2,'a',1);
  fa_add_transition(automate,2,'b',2);
  ASSERT_TRUE(fa_is_complete(automate));
  fa_destroy(automate);
}

TEST(AutomateTest, TestEstComplet_2) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,0,'b',2);
  fa_add_transition(automate,0,'b',0);
  fa_add_transition(automate,1,'a',1);
  fa_add_transition(automate,1,'b',0);
  fa_add_transition(automate,2,'a',1);
  fa_add_transition(automate,2,'b',2);
  ASSERT_TRUE(fa_is_complete(automate));
  fa_destroy(automate);
}


TEST(AutomateTest, TestEstComplet_3) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,0,'b',2);
  fa_add_transition(automate,1,'a',1);
  fa_add_transition(automate,1,'b',0);
  fa_add_transition(automate,2,'a',1);
  ASSERT_FALSE(fa_is_complete(automate));
  fa_destroy(automate);
}


 TEST(AutomateTest, TestCompletionAutomate) {
   struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
   fa_create(automate,2,3);
   fa_add_transition(automate,0,'a',1);
   fa_add_transition(automate,0,'b',2);
   fa_add_transition(automate,1,'a',1);
   fa_add_transition(automate,1,'b',0);
   fa_add_transition(automate,2,'a',1);
   ASSERT_FALSE(fa_is_complete(automate));
   fa_make_complete(automate);
   ASSERT_TRUE(fa_is_complete(automate));
   fa_destroy(automate);
 }

// //4.2
TEST(AutomateTest, TestParcoursProf_1) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  struct graph *gr = (struct graph*)malloc(sizeof(struct graph));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,1,'a',2);
  fa_add_transition(automate,2,'a',2);
  graph_create_from_fa(gr,automate,false);
  bool tab[automate->state_count];
  for (size_t i = 0; i < automate->state_count; i++) {
    tab[i] = false;
  }
  graph_depth_first_search(gr,0,tab);
  ASSERT_TRUE(tab[0]);
  ASSERT_TRUE(tab[1]);
  ASSERT_TRUE(tab[2]);
  for (size_t i = 0; i < automate->state_count; i++) {
    tab[i] = false;
  }
  graph_depth_first_search(gr,1,tab);
  ASSERT_FALSE(tab[0]);
  ASSERT_TRUE(tab[1]);
  ASSERT_TRUE(tab[2]);
  for (size_t i = 0; i < automate->state_count; i++) {
    tab[i] = false;
  }
  graph_depth_first_search(gr,2,tab);
  ASSERT_FALSE(tab[0]);
  ASSERT_FALSE(tab[1]);
  ASSERT_TRUE(tab[2]);
  fa_destroy(automate);
}

TEST(AutomateTest, TestParcoursProf_2) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  struct graph *gr = (struct graph*)malloc(sizeof(struct graph));
  fa_create(automate,2,3);
  graph_create_from_fa(gr,automate,false);
  bool tab[automate->state_count];
  for (size_t i = 0; i < automate->state_count; i++) {
    tab[i] = false;
  }
  ASSERT_NO_THROW(graph_depth_first_search(gr,0,tab));

  fa_destroy(automate);
}

TEST(AutomateTest, TestDetectionChemin) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  struct graph *gr = (struct graph*)malloc(sizeof(struct graph));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,1,'a',2);
  fa_add_transition(automate,2,'a',2);
  graph_create_from_fa(gr,automate,false);
  bool tab[automate->state_count];
  for (size_t i = 0; i < automate->state_count; i++) {
    tab[i] = false;
  }
  graph_depth_first_search(gr,0,tab);
  ASSERT_TRUE(tab[0]);
  ASSERT_TRUE(tab[1]);
  ASSERT_TRUE(tab[2]);
  for (size_t i = 0; i < automate->state_count; i++) {
    tab[i] = false;
  }
  graph_depth_first_search(gr,1,tab);
  ASSERT_FALSE(tab[0]);
  ASSERT_TRUE(tab[1]);
  ASSERT_TRUE(tab[2]);
  for (size_t i = 0; i < automate->state_count; i++) {
    tab[i] = false;
  }
  graph_depth_first_search(gr,2,tab);
  ASSERT_FALSE(tab[0]);
  ASSERT_FALSE(tab[1]);
  ASSERT_TRUE(tab[2]);
  fa_destroy(automate);
}

TEST(AutomateTest, TestDestrucGraph) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  struct graph *gr = (struct graph*)malloc(sizeof(struct graph));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,1,'a',2);
  fa_add_transition(automate,2,'a',2);
  graph_create_from_fa(gr,automate,false);
  ASSERT_NO_THROW(graph_destroy(gr));
  fa_destroy(automate);
}

TEST(AutomateTest, TestAccepteLVide_1) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,1,'a',2);
  fa_add_transition(automate,2,'a',2);
  ASSERT_TRUE(fa_is_language_empty(automate));
  fa_destroy(automate);
}

TEST(AutomateTest, TestAccepteLVide_2) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,1,1);
  fa_set_state_initial(automate,0);
  fa_set_state_final(automate,0);
  ASSERT_FALSE(fa_is_language_empty(automate));
  fa_destroy(automate);
}

TEST(AutomateTest, TestAccepteLVide_3) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  fa_create(automate,2,3);
  fa_set_state_initial(automate,0);
  fa_set_state_final(automate,2);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,1,'a',2);
  fa_add_transition(automate,2,'a',2);
  ASSERT_FALSE(fa_is_language_empty(automate));
  fa_destroy(automate);
}

TEST(AutomateTest, TestSupprEtatNonAcces) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  struct graph *gr = (struct graph*)malloc(sizeof(struct graph));
  fa_create(automate,2,4);
  bool tab[automate->state_count];
  for (size_t j = 0; j < automate->state_count; j++) {
    tab[j] = false;
  }
  fa_set_state_initial(automate,0);
  fa_set_state_final(automate,1);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,3,'a',1);
  fa_add_transition(automate,2,'a',2);
  fa_remove_non_accessible_states(automate);
  graph_create_from_fa(gr,automate,false);
  ASSERT_EQ(automate->state_count,2);
  for (size_t i = 0; i < automate->state_count; i++) {
    if (automate->states[i].is_initial) {
      for (size_t k = 0; k < automate->state_count; k++) {
        if (graph_has_path(gr,k,i)) {
          tab[k] = true;
        }
      }
    }
  }
  for (size_t r = 0; r < automate->state_count; r++) {
    ASSERT_TRUE(tab[r]);
  }
  free(gr);
  fa_destroy(automate);
}

TEST(AutomateTest, TestSupprEtatNonCoAcces) {
  struct fa *automate = (struct fa*)malloc(sizeof(struct fa));
  struct graph *gr = (struct graph*)malloc(sizeof(struct graph));
  fa_create(automate,2,5);
  bool tab[automate->state_count];
  for (size_t j = 0; j < automate->state_count; j++) {
    tab[j] = false;
  }
  fa_set_state_initial(automate,0);
  fa_set_state_final(automate,2);
  fa_add_transition(automate,0,'a',1);
  fa_add_transition(automate,1,'b',2);
  fa_add_transition(automate,1,'a',3);
  fa_add_transition(automate,2,'a',4);
      printf("////////////////////////////////////////1\n");
  fa_remove_non_co_accessible_states(automate);
        printf("////////////////////////////////////////2\n");
  graph_create_from_fa(gr,automate,false);
  ASSERT_EQ(automate->state_count,3);
  for (size_t i = 0; i < automate->state_count; i++) {
    if (automate->states[i].is_final) {
      for (size_t k = 0; k < automate->state_count; k++) {
        if (graph_has_path(gr,k,i)) {
          tab[k] = true;
        }
      }
    }
  }
  for (size_t r = 0; r < automate->state_count; r++) {
    ASSERT_TRUE(tab[r]);
  }
  free(gr);
  fa_destroy(automate);
}

// TEST(AutomateTest, TestCreatProduit) {
//
// }
//
// TEST(AutomateTest, TestIntersecVide) {
//
// }
//
// TEST(AutomateTest, TestCreationDeterministe) {
//
// }
//
// TEST(AutomateTest, TestEstInclus) {
//
// }
