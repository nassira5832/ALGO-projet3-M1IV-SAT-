
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

typedef struct {
    int *literals;  // Les littéraux de la clause
} clause;

typedef struct {
    int num_variables;
    int num_clauses;
    clause *clauses;
} formule;

// Fonction pour lire un fichier CNF
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
            F.num_clauses = num_clauses;
            F.clauses = (clause *)malloc(num_clauses * sizeof(clause));
            continue;
        }

        // Lire les clauses
        for (int i = 0; i < F.num_clauses; i++) {
            fgets(line, sizeof(line), f);
            F.clauses[i].literals = (int *)malloc(3 * sizeof(int));
            int j = 0;
            char *token = strtok(line, " ");
            while (token != NULL) {
                if (strcmp(token, "0") == 0) break;
                F.clauses[i].literals[j++] = atoi(token);
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(f);
    return F;
}

bool evaluer_clause(clause c, bool x1, bool x2, bool x3) {
    return (c.literals[0] && x1) || (c.literals[1] && x2) || (c.literals[2] && x3);
}

bool Solution(formule F, bool x1, bool x2, bool x3) {
    for (int i = 0; i < F.num_clauses; i++) {
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

double complexite (int k  , double t2 , double t1 ){
    double time = 0; 
    for (int i = 1; i <= k; i++) {
        time = time + (t2 - t1) / CLOCKS_PER_SEC;
    }
    return time / k; 
}

int main() {
    srand(time(NULL)); // Initialisation du générateur aléatoire
    
    // Ouvrir le dossier contenant les fichiers CNF
    
    DIR *d = opendir("UUF75.325.100");

    if (d == NULL) {
        printf("Erreur d'ouverture du dossier\n");
        return 1;
    }

    struct dirent *entry;
    FILE *F = fopen("resultat3SAT_trv_Sol.csv", "w");
    fprintf(F, "fichier,temps,memUsage\n");

    // Parcourir tous les fichiers du dossier
    while ((entry = readdir(d)) != NULL) {
        // Ignorer les fichiers spéciaux "." et ".."
        if (entry->d_name[0] == '.') continue;

        char filepath[256];
        snprintf(filepath, sizeof(filepath), "UUF75.325.100/%s", entry->d_name);

        double t1 = clock();
        
        // Lire le fichier CNF
        formule Fm = lireCNF(filepath); 
        
        bool x1, x2, x3; 
        trouver_solution(Fm, &x1, &x2, &x3);

        // Libérer la mémoire allouée pour les clauses
        for (int i = 0; i < Fm.num_clauses; i++) {
            free(Fm.clauses[i].literals);
        }
        free(Fm.clauses);

        double t2 = clock();
        double temps = complexite(1, t2, t1); // Vous pouvez ajuster 'k' si nécessaire
        size_t memoire = Fm.num_clauses * sizeof(clause);
        fprintf(F, "%s,%f,%zu\n", entry->d_name, temps, memoire);
    }

    fclose(F);
    closedir(d);
    return 0;
}
