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

double complexite(int k, double t2, double t1) {
    return (t2 - t1) / (CLOCKS_PER_SEC * k); // Calcul du temps d'exécution moyen
}

void generer_solution(int num_vars, bool* solution) {
    for (int i = 0; i < num_vars; i++) {
        solution[i] = rand() % 2;  
    }
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
    FILE *F = fopen("resultatSAT_ver_Sol.csv", "w");

    if (F == NULL) {
        printf("Erreur d'ouverture du fichier CSV\n");
        return 1;
    }
    int i =0 ; 
    fprintf(F, "nbr_clauses,num_vars,num_literals,temps,memUsage\n");

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "Bejing/%s", entry->d_name);
        
        // Allocation de la mémoire pour l'assignation des variables
        Formula formula = lireCNF(filepath);
        bool *assignment = (bool *)malloc(formula.num_vars * sizeof(bool)); // Allocation dynamique

        // Générer une solution aléatoire
        generer_solution(formula.num_vars, assignment);

        double t1 = clock();
        bool result = verify_solution(&formula, assignment);
        double t2 = clock(); 
        double temps = complexite(k, t2, t1); // Calcul du temps d'exécution
        
        // Calcul de la mémoire utilisée
        size_t memoire = sizeof(formula) + formula.num_clauses * sizeof(Clause);
        for (int i = 0; i < formula.num_clauses; i++) {
            memoire += formula.clauses[i].num_literals * sizeof(int);
        }
        
        // Enregistrement des résultats dans le fichier CSV
        fprintf(F, "%d,%d,%d,%f,%zu\n", formula.num_clauses, formula.num_vars, formula.clauses[0].num_literals, temps, memoire);
        
        // Libération de la mémoire allouée
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
