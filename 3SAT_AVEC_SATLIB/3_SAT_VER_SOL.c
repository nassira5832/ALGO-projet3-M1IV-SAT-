#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

typedef struct {
    int *literals;  // Tableau des littéraux pour chaque clause
} clause;

typedef struct {
    int num_variables;  // Nombre de variables
    int nbr_clauses;    // Nombre de clauses
    clause *clauses;    // Tableau des clauses
} formule;

clause generer_clause() {
    clause c;
    c.literals = (int *)malloc(3 * sizeof(int));  // Allouer 3 littéraux pour la clause
    for (int i = 0; i < 3; i++) {
        c.literals[i] = rand() % 2 ? i + 1 : -(i + 1);  // Littéral aléatoire entre 1 et 3
    }
    return c;
}

formule lireCNF(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier %s\n", filename);
        exit(1);
    }

    formule F;
    char line[1024];
    
    while (fgets(line, sizeof(line), f)) {
        // Ignorer les commentaires
        if (line[0] == 'c') continue;

        // Lire la ligne de problème
        if (line[0] == 'p') {
            int num_variables, num_clauses;
            sscanf(line, "p cnf %d %d", &num_variables, &num_clauses);
            F.num_variables = num_variables;
            F.nbr_clauses = num_clauses;
            F.clauses = (clause *)malloc(num_clauses * sizeof(clause));
            continue;
        }

        // Lire les clauses
        int clause_index = 0;
        while (fgets(line, sizeof(line), f) && clause_index < F.nbr_clauses) {
            F.clauses[clause_index].literals = (int *)malloc(3 * sizeof(int));  // Allocation pour 3 littéraux
            int j = 0;
            char *token = strtok(line, " ");
            while (token != NULL && j < 3) {
                if (strcmp(token, "0") == 0) break;  // Fin de la clause
                F.clauses[clause_index].literals[j++] = atoi(token);
                token = strtok(NULL, " ");
            }
            clause_index++;
        }
    }

    fclose(f);
    return F;
}

bool evaluer_clause(clause c, bool x1, bool x2, bool x3) {
    bool res = false;
    for (int i = 0; i < 3; i++) {
        int literal = c.literals[i];
        bool val = (literal > 0) ? (literal == 1 ? x1 : (literal == 2 ? x2 : x3)) : (literal == -1 ? !x1 : (literal == -2 ? !x2 : !x3));
        res = res || val;  // OR logique
    }
    return res;
}

bool verifier_sol(formule F, bool x1, bool x2, bool x3) {
    for (int i = 0; i < F.nbr_clauses; i++) {
        if (!evaluer_clause(F.clauses[i], x1, x2, x3)) {
            return false;  // Si une clause n'est pas satisfaite
        }
    }
    return true;  // Toutes les clauses sont satisfaites
}

double complexite(int k, double t2, double t1) {
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
    DIR *d = opendir("UUF75.325.100");
    if (d == NULL) {
        printf("Erreur d'ouverture du dossier\n");
        return 1;
    }

    struct dirent *entry;
    FILE *file = fopen("resultat_ver_sol.csv", "w");
    fprintf(file, "nbr_clauses,temps,memUsage\n");

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue;  // Ignorer les fichiers cachés

        char filepath[256];
        snprintf(filepath, sizeof(filepath), "UUF75.325.100/%s", entry->d_name);

        double t1 = clock();
        formule Fm = lireCNF(filepath);

        // Générer une solution aléatoire et vérifier
        bool x1 = rand() % 2;
        bool x2 = rand() % 2;
        bool x3 = rand() % 2;
        verifier_sol(Fm, x1, x2, x3);

        double t2 = clock();
        double temps = complexite(k, t2, t1);
        size_t memoire = sizeof(Fm) + (Fm.nbr_clauses * sizeof(clause));

        fprintf(file, "%d,%f,%zu\n", Fm.nbr_clauses, temps, memoire);
    }

    closedir(d);
    fclose(file);
    return 0;
}
