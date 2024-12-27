#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    bool *x;
    int nbr_litterals;
} clause;

typedef struct {
    int nbr_clauses;
    clause *clauses;
} formule;

clause generer_clause(int nbr_litterals) {
    clause c;
    c.x=(bool *)malloc(nbr_litterals * sizeof(bool));
    for (int i= 0 ; i<nbr_litterals){
        c.x[i]=rand() % 2;
    }
    return c;
}

formule generer_Formule(int nbr_clauses, int nbr_litterals) {
    formule F;
    F.nbr_clauses = nbr_clauses;
    F.clauses = (clause *)malloc(nbr_clauses * sizeof(clause)); // Allocation dynamique de mémoire
    for (int i = 0; i < nbr_clauses; i++) {
        F.clauses[i] = generer_clause(nbr_litterals);
    }
    return F;
}

bool evaluer_clause(clause c, bool *x) {
    bool a= true  ; 
    for (int i=0 ; i<c.nbr_litterals; i++){
      a= a|| (c.x[i] && x[i]);
    }
    return a;
}

