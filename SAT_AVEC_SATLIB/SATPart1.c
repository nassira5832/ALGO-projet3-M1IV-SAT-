#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

typedef struct {
    int *literals;
    int num_literals;
} Clause;

typedef struct {
    Clause *clauses;
    int num_clauses;
    int num_vars;
} Formula;

Formula lireCNF(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier %s\n", filename);
        exit(1);
    }

    Formula F;
    char line[1024];
    F.num_vars = 0;
    F.num_clauses = 0;
    F.clauses = NULL;
    
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == 'c') continue;

        if (line[0] == 'p') {
            int num_variables, num_clauses;
            sscanf(line, "p cnf %d %d", &num_variables, &num_clauses);
            F.num_vars = num_variables;
            F.num_clauses = num_clauses;
            F.clauses = (Clause *)malloc(num_clauses * sizeof(Clause));
            for (int i = 0; i < num_clauses; i++) {
                F.clauses[i].literals = NULL;
                F.clauses[i].num_literals = 0;
            }
            continue;
        }

        int current_clause = -1;
        char *token = strtok(line, " ");
        while (token != NULL) {
            if (strcmp(token, "0") == 0) break;
            int literal = atoi(token);

            if (current_clause == -1 || F.clauses[current_clause].literals == NULL) {
                current_clause++;
            }

            F.clauses[current_clause].literals = (int *)realloc(F.clauses[current_clause].literals, (F.clauses[current_clause].num_literals + 1) * sizeof(int));
            F.clauses[current_clause].literals[F.clauses[current_clause].num_literals++] = literal;
            token = strtok(NULL, " ");
        }
    }

    fclose(f);
    return F;
}

bool dpll(Formula *formula, bool *assignment, int var_index) {
    // Implémenter l'algorithme DPLL
    // Remarque : La pseudo-implémentation de DPLL n'est pas fournie ici, il s'agit d'une simplification.
    // Pour une implémentation complète, des fonctionnalités telles que l'unité de propagation et les heuristiques de choix de variables sont nécessaires.
    return false;
}

void free_formula(Formula *formula) {
    for (int i = 0; i < formula->num_clauses; i++) {
        free(formula->clauses[i].literals); 
    }
    free(formula->clauses); 
}

double complexite (int k, double t2, double t1) {
    return (t2 - t1) / CLOCKS_PER_SEC;
}

int main() {
    srand(time(NULL));
    int k = 200;
    DIR *d = opendir("QG");
    if (d == NULL) {
        printf("Erreur d'ouverture du dossier\n");
        return 1;
    }

    struct dirent *entry;
    FILE *F = fopen("resultatSAT_trv_Sol.csv", "w");
    if (F == NULL) {
        printf("Erreur d'ouverture du fichier CSV\n");
        return 1;
    }
    fprintf(F, "nbr_clauses,num_vars,num_literals,temps,memUsage\n");

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "QG/%s", entry->d_name);
        
        Formula Fm = lireCNF(filepath);
        bool *assignment = malloc(Fm.num_vars * sizeof(bool));
        
        double t1 = clock();

        bool result = dpll(&Fm, assignment, 0);
        double t2 = clock(); 
        double temps = complexite(k, t2, t1);

        size_t memoire = Fm.num_clauses * sizeof(Clause) + Fm.num_vars * sizeof(int);

        fprintf(F, "%d,%d,%d,%f,%zu\n", Fm.num_clauses, Fm.num_vars, Fm.clauses[0].num_literals, temps, memoire);

        free(assignment);
        free_formula(&Fm);
    }

    fclose(F);
    closedir(d);
    return 0;
}
