#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

typedef struct {
    int *literals;        // Tableau des littéraux de la clause
    int num_literals;     // Nombre de littéraux dans la clause
} Clause;

typedef struct {
    Clause *clauses;      // Tableau des clauses
    int num_clauses;      // Nombre de clauses
    int num_vars;         // Nombre de variables
} Formula;

Formula lireCNF(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier %s\n", filename);
        exit(1);
    }

    Formula F;
    char line[1024];
    
    while (fgets(line, sizeof(line), f)) {
        // Ignorer les commentaires
        if (line[0] == 'c') continue;

        // Lire la ligne de problème
        if (line[0] == 'p') {
            int num_variables, num_clauses;
            sscanf(line, "p cnf %d %d", &num_variables, &num_clauses);
            F.num_vars = num_variables;
            F.num_clauses = num_clauses;
            F.clauses = (Clause *)malloc(num_clauses * sizeof(Clause));
            continue;
        }

        // Lire les clauses
        for (int i = 0; i < F.num_clauses; i++) {
            fgets(line, sizeof(line), f);
            F.clauses[i].literals = (int *)malloc(3 * sizeof(int)); // Réserver de la place pour 3 littéraux au maximum
            int j = 0;
            char *token = strtok(line, " ");
            while (token != NULL) {
                if (strcmp(token, "0") == 0) break;
                F.clauses[i].literals[j++] = atoi(token);
                token = strtok(NULL, " ");
            }
            F.clauses[i].num_literals = j; // Mettre à jour le nombre réel de littéraux
        }
    }

    fclose(f);
    return F;
}

bool evaluate_clause(Clause *clause, bool *assignment) {
    for (int i = 0; i < clause->num_literals; i++) {
        int literal = clause->literals[i];
        int var = abs(literal) - 1; // Les indices commencent à 0
        bool value = assignment[var];

        if (literal < 0) {
            value = !value; // Négation si littéral négatif
        }

        if (value) {
            return true; // La clause est satisfaite
        }
    }
    return false; // La clause n'est pas satisfaite
}

bool evaluate_formula(Formula *formula, bool *assignment) {
    for (int i = 0; i < formula->num_clauses; i++) {
        if (!evaluate_clause(&formula->clauses[i], assignment)) {
            return false; // Une clause n'est pas satisfaite
        }
    }
    return true; // Toutes les clauses sont satisfaites
}

bool solve_sat_recursive(Formula *formula, bool *assignment, int var_index) {
    if (var_index == formula->num_vars) {
        return evaluate_formula(formula, assignment);
    }

    // Essayer les deux valeurs : faux puis vrai
    assignment[var_index] = false;
    if (solve_sat_recursive(formula, assignment, var_index + 1)) {
        return true;
    }

    assignment[var_index] = true;
    if (solve_sat_recursive(formula, assignment, var_index + 1)) {
        return true;
    }

    return false; // Aucune solution trouvée
}

bool solve_sat(Formula *formula, bool *assignment) {
    return solve_sat_recursive(formula, assignment, 0);
}

void free_formula(Formula *formula) {
    for (int i = 0; i < formula->num_clauses; i++) {
        free(formula->clauses[i].literals); 
    }
    free(formula->clauses); 
}

// Fonction pour calculer le temps de complexité (en utilisant un simple calcul)
double complexite (int k, double t2, double t1) {
    double time = 0; 
    double T;
    for (int i = 1; i <= k; i++) {
       time = time + (t2 - t1) / CLOCKS_PER_SEC;
    }
    T = time / k;
    return T; 
}

int main() {
    srand(time(NULL));  // Initialisation du générateur aléatoire
    int k = 200;
    DIR *d = opendir("Bejing");
    if (d == NULL) {
        printf("Erreur d'ouverture du dossier\n");
        return 1;
    }

    struct dirent *entry;
    FILE *F = fopen("resultatSAT2_trv_Sol.csv", "w");
    fprintf(F, "nbr_clauses,num_vars,num_literals,temps,memUsage\n");

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "Bejing/%s", entry->d_name);
        
        // Allocation de la mémoire pour l'assignation des variables
        Formula Fm = lireCNF(filepath);
        bool *assignment = malloc(Fm.num_vars * sizeof(bool));

        double t1 = clock();
        bool result = solve_sat(&Fm, assignment);
        double t2 = clock(); 
        double temps = complexite(k, t2, t1);

        size_t memoire = Fm.num_clauses * sizeof(Clause) + Fm.num_vars * sizeof(int); // Estimation de la mémoire utilisée

        fprintf(F, "%d,%d,%d,%f,%zu\n", Fm.num_clauses, Fm.num_vars, Fm.clauses[0].num_literals, temps, memoire);

        free(assignment);
        free_formula(&Fm);
    }

    fclose(F);
    return 0;
}
