#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VARS 100
#define MAX_CLAUSES 100
#define MAX_LITERALS_PER_CLAUSE 100

typedef struct {
    int literals[MAX_LITERALS_PER_CLAUSE];
    int num_literals;
} Clause;

typedef struct {
    Clause clauses[MAX_CLAUSES];
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

int main() {
    Formula formula;
    bool assignment[MAX_VARS];

    printf("Entrez le nombre de variables : ");
    scanf("%d", &formula.num_vars);
    printf("Entrez le nombre de clauses : ");
    scanf("%d", &formula.num_clauses);

    for (int i = 0; i < formula.num_clauses; i++) {
        printf("\nPour la clause %d:\n", i + 1);
        printf("Entrez le nombre de littéraux : ");
        scanf("%d", &formula.clauses[i].num_literals);

        printf("Entrez les littéraux (nombre positif pour x, négatif pour ¬x) : ");
        for (int j = 0; j < formula.clauses[i].num_literals; j++) {
            scanf("%d", &formula.clauses[i].literals[j]);
        }
    }

    printf("\nEntrez la solution proposée (0 pour faux, 1 pour vrai) :\n");
    for (int i = 0; i < formula.num_vars; i++) {
        printf("x%d = ", i + 1);
        int val;
        scanf("%d", &val);
        assignment[i] = (val != 0);
    }

    printf("\nVérification de la solution pour la formule :\n");
    print_formula(&formula);

    printf("\nSolution proposée :\n");
    for (int i = 0; i < formula.num_vars; i++) {
        printf("x%d = %s\n", i + 1, assignment[i] ? "vrai" : "faux");
    }
    
    printf("\nRésultat de la vérification :\n");
    if (verify_solution(&formula, assignment)) {
        printf("La solution satisfait toutes les clauses!\n");
    } else {
        printf("La solution ne satisfait pas toutes les clauses!\n");
    }

    return 0;
}
