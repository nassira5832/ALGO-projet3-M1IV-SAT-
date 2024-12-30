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

bool evaluate_clause(Clause* clause, bool* assignment) {
    for (int i = 0; i < clause->num_literals; i++) {
        int literal = clause->literals[i];
        int var = abs(literal);
        bool value = assignment[var - 1];

        if (literal < 0) {
            value = !value;
        }

        if (value) {
            return true;
        }
    }
    return false;
}

bool evaluate_formula(Formula* formula, bool* assignment) {
    for (int i = 0; i < formula->num_clauses; i++) {
        if (!evaluate_clause(&formula->clauses[i], assignment)) {
            return false;
        }
    }
    return true;
}

bool solve_sat_recursive(Formula* formula, bool* assignment, int var_index) {
    if (var_index == formula->num_vars) {
        return evaluate_formula(formula, assignment);
    }

    assignment[var_index] = false;
    if (solve_sat_recursive(formula, assignment, var_index + 1)) {
        return true;
    }

    assignment[var_index] = true;
    if (solve_sat_recursive(formula, assignment, var_index + 1)) {
        return true;
    }

    return false;
}

bool solve_sat(Formula* formula, bool* assignment) {
    return solve_sat_recursive(formula, assignment, 0);
}

void read_formula(Formula* formula) {
    printf("Entrez le nombre de variables : ");
    scanf("%d", &formula->num_vars);

    printf("Entrez le nombre de clauses : ");
    scanf("%d", &formula->num_clauses);

    for (int i = 0; i < formula->num_clauses; i++) {
        printf("Entrez le nombre de littéraux pour la clause %d : ", i + 1);
        scanf("%d", &formula->clauses[i].num_literals);

        printf("Entrez les littéraux (nombre positif pour x, négatif pour ¬x) : ");
        for (int j = 0; j < formula->clauses[i].num_literals; j++) {
            scanf("%d", &formula->clauses[i].literals[j]);
        }
    }
}

int main() {
    Formula formula;
    bool assignment[MAX_VARS];

    read_formula(&formula);

    if (solve_sat(&formula, assignment)) {
        printf("SAT: La formule est satisfaisable\n");
        printf("Une solution possible est :\n");
        for (int i = 0; i < formula.num_vars; i++) {
            printf("x%d = %s\n", i + 1, assignment[i] ? "vrai" : "faux");
        }
    } else {
        printf("UNSAT: La formule n'est pas satisfaisable\n");
    }

    return 0;
}
