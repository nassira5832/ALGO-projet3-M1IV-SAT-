#include <stdlib.h>
#include <stdio.h>
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

bool Solution(formule F, bool x1, bool x2, bool x3) {
    for (int i = 0; i < F.nbr_clauses; i++) {
        if (!evaluer_clause(F.clauses[i], x1, x2, x3)) {
            return false;
        }
    }
    return true;
}

bool trouver_solution(formule F, bool *sol_x1, bool *sol_x2, bool *sol_x3) {
    bool solutions[8][3] = {
        {true, true, true}, {true, true, false}, {true, false, true}, {true, false, false},
        {false, true, true}, {false, true, false}, {false, false, true}, {false, false, false}
    };
    for (int i = 0; i < 8; i++) {
        if (Solution(F, solutions[i][0], solutions[i][1], solutions[i][2])) {
            *sol_x1 = solutions[i][0];
            *sol_x2 = solutions[i][1];
            *sol_x3 = solutions[i][2];
            return true;
        }
    }
    return false;
}

int main() {
    srand(time(NULL)); // Initialisation du générateur aléatoire

    formule F = generer_Formule(3); 
    bool x1, x2, x3;

    if (trouver_solution(F, &x1, &x2, &x3)) {
        printf("Solution trouvée : x1=%s, x2=%s, x3=%s\n",
               x1 ? "true" : "false",
               x2 ? "true" : "false",
               x3 ? "true" : "false");
    } else {
        printf("Pas de solution.\n");
    }

    free(F.clauses); 
    return 0;
}