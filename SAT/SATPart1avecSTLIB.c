#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "satlib.h" // Inclure SATLIB (assurez-vous que le chemin est correct)

typedef struct {
    int *literals;        // Tableau des littéraux de la clause
    int num_literals;     // Nombre de littéraux dans la clause
} Clause;

typedef struct {
    Clause *clauses;      // Tableau des clauses
    int num_clauses;      // Nombre de clauses
    int num_vars;         // Nombre de variables
} Formula;

bool solve_sat_satlib(Formula *formula) {
    // Initialiser une instance SATLIB
    sat_solver *solver = sat_solver_new();

    // Ajouter les clauses à SATLIB
    for (int i = 0; i < formula->num_clauses; i++) {
        lit *clause = malloc(formula->clauses[i].num_literals * sizeof(lit));
        for (int j = 0; j < formula->clauses[i].num_literals; j++) {
            int literal = formula->clauses[i].literals[j];
            clause[j] = (literal > 0) ? to_lit(literal - 1) : to_lit(-(literal + 1));
        }
        sat_solver_add_clause(solver, clause, formula->clauses[i].num_literals);
        free(clause);
    }

    // Résoudre le problème SAT
    int result = sat_solver_solve(solver);
    sat_solver_free(solver);

    // Retourner si le problème est satisfiable
    return (result == SAT_SOLVABLE);
}

void read_formula(Formula *formula) {
    printf("Entrez le nombre de variables : ");
    scanf("%d", &formula->num_vars);

    printf("Entrez le nombre de clauses : ");
    scanf("%d", &formula->num_clauses);

    formula->clauses = malloc(formula->num_clauses * sizeof(Clause));

    for (int i = 0; i < formula->num_clauses; i++) {
        printf("Entrez le nombre de littéraux pour la clause %d : ", i + 1);
        scanf("%d", &formula->clauses[i].num_literals);

        formula->clauses[i].literals = malloc(formula->clauses[i].num_literals * sizeof(int));

        printf("Entrez les littéraux (nombre positif pour x, négatif pour ¬x) : ");
        for (int j = 0; j < formula->clauses[i].num_literals; j++) {
            scanf("%d", &formula->clauses[i].literals[j]);
        }
    }
}

void free_formula(Formula *formula) {
    for (int i = 0; i < formula->num_clauses; i++) {
        free(formula->clauses[i].literals); 
    }
    free(formula->clauses); 
}

// Fonction pour calculer le temps de complexité (en utilisant un simple calcul)
double complexite(int k, double t2, double t1) {
    double time = 0;
    double T;
    for (int i = 1; i <= k; i++) {
        time = time + (t2 - t1) / CLOCKS_PER_SEC;
    }
    T = time / k;
    return T;
}

Formula generer_Formule(int num_clauses) {
    Formula formule;
    formule.num_clauses = num_clauses;
    formule.num_vars = rand() % 50 + 1; // Le nombre de variables varie de 1 à 50

    formule.clauses = malloc(formule.num_clauses * sizeof(Clause));
    
    for (int i = 0; i < formule.num_clauses; i++) {
        formule.clauses[i].num_literals = rand() % 3 + 1; // Chaque clause a entre 1 et 3 littéraux
        formule.clauses[i].literals = malloc(formule.clauses[i].num_literals * sizeof(int));
        
        for (int j = 0; j < formule.clauses[i].num_literals; j++) {
            formule.clauses[i].literals[j] = rand() % 2 ? j + 1 : -(j + 1); // Littéraux positifs ou négatifs
        }
    }
    return formule;
}

int main() {
    srand(time(NULL)); 

    int start = 2;
    int end = 16;
    int Step = 2;
    int k = 100;

    FILE *F = fopen("resultatSAT2_trv_Sol.csv", "w");
    fprintf(F, "nbr_clauses,num_vars,num_literals,temps,memUsage\n");

    while (start < end) {
        printf("%d\n", start);

        Formula formule = generer_Formule(start);
        double t1 = clock();
        bool result = solve_sat_satlib(&formule);  // Utilisation du solveur SATLIB
        double t2 = clock(); 
        double temps = complexite(k, t2, t1);

        size_t memoire = formule.num_clauses * sizeof(Clause) + formule.num_clauses * 3 * sizeof(int); // Estimation de la mémoire utilisée

        fprintf(F, "%d,%d,%d,%f,%zu\n", formule.num_clauses, formule.num_vars, formule.clauses[0].num_literals, temps, memoire);

        free_formula(&formule);

        start = start + Step;
    }

    fclose(F);
    return 0;
}
