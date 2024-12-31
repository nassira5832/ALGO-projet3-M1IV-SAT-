#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int *literals;
    int num_literals;
} Clause;

typedef struct {
    Clause *clauses;
    int num_clauses;
    int num_vars;
} Formula;

bool is_clause_satisfied(Clause* clause, bool* assignment) {
    for (int i = 0; i < clause->num_literals; i++) {
        int literal = clause->literals[i];
        int var_index = abs(literal) - 1;
        bool value = assignment[var_index];

        if (literal < 0) {
            value = !value;
        }

        if (value) {
            return true;
        }
    }
    return false;
}

bool verify_solution(Formula* formula, bool* assignment) {
    for (int i = 0; i < formula->num_clauses; i++) {
        if (!is_clause_satisfied(&formula->clauses[i], assignment)) {
            printf("La clause %d n'est pas satisfaite!\n", i + 1);
            return false;
        }
    }
    return true;
}

void print_clause(Clause* clause) {
    printf("(");
    for (int i = 0; i < clause->num_literals; i++) {
        if (clause->literals[i] < 0) {
            printf("¬x%d", abs(clause->literals[i]));
        } else {
            printf("x%d", clause->literals[i]);
        }
        if (i < clause->num_literals - 1) {
            printf(" ∨ ");
        }
    }
    printf(")");
}

void print_formula(Formula* formula) {
    printf("F = ");
    for (int i = 0; i < formula->num_clauses; i++) {
        print_clause(&formula->clauses[i]);
        if (i < formula->num_clauses - 1) {
            printf(" ∧ ");
        }
    }
    printf("\n");
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


void generer_solution(int num_vars, bool* solution) {
    for (int i = 0; i < num_vars; i++) {
        solution[i] = rand() % 2;  
    }
}

int main() {
    srand(time(NULL)); 

    int start = 2000000;
    int end = 4000000;
    int Step = 2000;
    int k =20000; 

    FILE *F = fopen("resultatSAT_ver_Sol.csv", "w");

    fprintf(F, "num_clauses,num_vars,num_literals,temps\n");

    while (start < end) {
        int num_clauses = start;  
        Formula formula = generer_Formule(num_clauses);
        bool assignment[100];

        // Générer une solution aléatoire
        generer_solution(formula.num_vars, assignment);

        
        double t1 = clock();
        bool result = verify_solution(&formula, assignment);

        double t2 = clock(); 
        double temps = complexite(k, t2, t1); // Calcul du temps d'exécution

        // Enregistrement des résultats dans le fichier CSV
        fprintf(F, "%d,%d,%d,%f\n", num_clauses, formula.num_vars, formula.clauses->num_literals, temps);

        // Libération de la mémoire allouée
        for (int i = 0; i < formula.num_clauses; i++) {
            free(formula.clauses[i].literals);
        }
        free(formula.clauses);

        start = start + Step;  
    }

    fclose(F);  
    return 0;
}
