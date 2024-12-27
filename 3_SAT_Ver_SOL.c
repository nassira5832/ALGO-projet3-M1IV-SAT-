#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    bool x1, x2, x3;
} clause;

typedef struct {
    int nbr_clauses;
    clause *clauses;
} formule;

clause generer_clause() {
    clause c;
    c.x1 = rand() % 2; 
    c.x2 = rand() % 2; 
    c.x3 = rand() % 2;
    return c;
}

formule generer_Formule(int nbr_clauses) {
    formule F;
    F.nbr_clauses = nbr_clauses;
    F.clauses = (clause *)malloc(nbr_clauses * sizeof(clause)); // Allocation dynamique de mémoire
    for (int i = 0; i < nbr_clauses; i++) {
        F.clauses[i] = generer_clause();
    }
    return F;
}

bool evaluer_clause(clause c, bool x1, bool x2, bool x3) {
    return (c.x1 && x1) || (c.x2 && x2) || (c.x3 && x3);
}

bool verefier_sol (formule F , clause c ){
    for (int i=0 ; i<F.nbr_clauses ; i++){
        if(!evaluer_clause(F.clauses[i],c.x1 ,c.x2 ,c.x3)){
            return false;
        }
    }
    return true;
}
int main() {
    formule F = generer_Formule(3);
    clause solution = generer_clause();
    if (verefier_sol(F, solution)){
        printf("la solution donnée est correct");
    }else{
        printf("la solution donnée est fausse ");
    }
    return 0 ; 
}
 
