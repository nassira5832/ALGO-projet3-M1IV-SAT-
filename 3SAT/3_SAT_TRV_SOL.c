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
bool trouver_solution(formule F, bool *sol_x) {
    int total_combinations = 1 << 3; // 2^3 = 8 combinaisons possibles
    for (int i = 0; i < total_combinations; i++) {
        bool valeurs[3] = {
            (i & 1) != 0, // Bit 0
            (i & 2) != 0, // Bit 1
            (i & 4) != 0  // Bit 2
        };
        if (evaluer_formule(F, valeurs)) {
            for (int j = 0; j < 3; j++) {
                sol_x[j] = valeurs[j];
            }
            return true;
        }
    }
    return false;
}
double complexite (int k  , double t2 , double t1 ){
    double time=0; 
    double T;
    for (int i=1 ; i<=k ; i++){
       time= time+(t2 - t1)/CLOCKS_PER_SEC;
    }
    T=time/k;
    return T ; 
}
int main() {
    srand(time(NULL)); // Initialisation du générateur aléatoire
    int start=2;
    int end = 40000;
    int Step =2 ; 
    int k=20000; 
    FILE *F = fopen("resultat3SAT_trv_Sol.csv", "w");
    fprintf(F, "nbr_clauses,temps,memUsage\n");
   

    while (start<end){

        printf ("%d",start); 
        double  t1= clock();
        formule formule = generer_Formule(start); 
        bool x1, x2, x3; 
        free(formule.clauses);
        double  t2= clock();
        double temps = complexite(k,t2,t1);
        size_t memoire = formule.nbr_clauses * sizeof(clause);
        fprintf(F,"%d,%f,%zu\n",start,temps , memoire);
        start=start+Step;
    } 
    return 0;
}