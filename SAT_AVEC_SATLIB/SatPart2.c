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

        if (F.num_clauses > 0 && F.clauses != NULL) {
            int i = 0;
            for (i = 0; i < F.num_clauses; i++) {
                if (F.clauses[i].literals == NULL) break;
            }
            
            int num_literals = 0;
            char *token = strtok(line, " ");
            while (token != NULL) {
                if (strcmp(token, "0") == 0) break;
                num_literals++;
                token = strtok(NULL, " ");
            }

            F.clauses[i].literals = (int *)malloc(num_literals * sizeof(int));
            F.clauses[i].num_literals = num_literals;

            token = strtok(line, " ");
            for (int j = 0; j < num_literals; j++) {
                F.clauses[i].literals[j] = atoi(token);
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(f);
    return F;
}

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

void generer_solution(int num_vars, bool* solution) {
    for (int i = 0; i < num_vars; i++) {
        solution[i] = rand() % 2;
    }
}

double complexite(int k, double t2, double t1) {
    return (t2 - t1) / (CLOCKS_PER_SEC * k);
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
    FILE *F = fopen("resultatSAT2_ver_Sol.csv", "w");

    if (F == NULL) {
        printf("Erreur d'ouverture du fichier CSV\n");
        return 1;
    }
    fprintf(F, "nbr_clauses,num_vars,num_literals,temps,memUsage\n");

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "QG/%s", entry->d_name);
        printf("Traitement du fichier : %s\n", filepath);

        Formula formula = lireCNF(filepath);
        printf("Nombre de clauses: %d, Nombre de variables: %d\n", formula.num_clauses, formula.num_vars);
        if (formula.num_clauses == 0 || formula.num_vars == 0) {
            printf("Erreur: Fichier CNF vide ou incorrect : %s\n", filepath);
            continue;
        }

        bool *assignment = (bool *)malloc(formula.num_vars * sizeof(bool));

        generer_solution(formula.num_vars, assignment);

        double t1 = clock();
        bool result = verify_solution(&formula, assignment);
        double t2 = clock(); 
        double temps = complexite(k, t2, t1);
        printf("Temps d'exécution: %f\n", temps);

        size_t memoire = sizeof(formula) + formula.num_clauses * sizeof(Clause);
        for (int i = 0; i < formula.num_clauses; i++) {
            memoire += formula.clauses[i].num_literals * sizeof(int);
        }
        printf("Mémoire utilisée: %zu\n", memoire);
        
        fprintf(F, "%d,%d,%d,%f,%zu\n", formula.num_clauses, formula.num_vars, formula.clauses[0].num_literals, temps, memoire);
        fflush(F);

        for (int i = 0; i < formula.num_clauses; i++) {
            free(formula.clauses[i].literals);
        }
        free(formula.clauses);
        free(assignment);
    }

    fclose(F);
    closedir(d);
    return 0;
}
