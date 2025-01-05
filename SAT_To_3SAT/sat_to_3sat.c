#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VARS 1000
#define MAX_CLAUSES 1000
#define MAX_LITERALS_PER_CLAUSE 100

// Structures existantes
typedef struct {
    int literals[MAX_LITERALS_PER_CLAUSE];
    int num_literals;
} Clause;

typedef struct {
    Clause clauses[MAX_CLAUSES];
    int num_clauses;
    int num_vars;
} Formula;

typedef struct {
    int literals[3];
} Clause3SAT;

typedef struct {
    Clause3SAT* clauses;
    int num_clauses;
    int num_vars;
} Formula3SAT;

typedef struct {
    size_t total_memory;
    size_t original_memory;
    size_t result_memory;
    double time_taken;
} Complexity;

// Fonction pour générer une formule SAT aléatoire
Formula* generate_random_sat(int num_vars, int num_clauses, int max_literals_per_clause) {
    Formula* formula = (Formula*)malloc(sizeof(Formula));
    if (formula == NULL) {
        printf("Erreur d'allocation mémoire pour la formule\n");
        exit(1);
    }

    formula->num_vars = num_vars;
    formula->num_clauses = num_clauses;

    printf("\nGénération d'une formule SAT avec %d variables et %d clauses\n", num_vars, num_clauses);

    for(int i = 0; i < num_clauses; i++) {
        int literals_count = 1 + rand() % max_literals_per_clause;
        formula->clauses[i].num_literals = literals_count;

        printf("Clause %d (%d littéraux): ", i+1, literals_count);
        for(int j = 0; j < literals_count; j++) {
            int var = 1 + rand() % num_vars;
            formula->clauses[i].literals[j] = (rand() % 2) ? var : -var;
            printf("%d ", formula->clauses[i].literals[j]);
        }
        printf("\n");
    }
    return formula;
}

// Fonction de transformation d'une clause longue
void transform_long_clause_linear(Clause* input, Formula3SAT* output, int* current_clause, int* new_var) {
    int k = input->num_literals;

    output->clauses[*current_clause].literals[0] = input->literals[0];
    output->clauses[*current_clause].literals[1] = input->literals[1];
    output->clauses[*current_clause].literals[2] = *new_var;
    (*current_clause)++;

    for(int i = 2; i < k-1; i++) {
        output->clauses[*current_clause].literals[0] = -(*new_var);
        output->clauses[*current_clause].literals[1] = input->literals[i];
        output->clauses[*current_clause].literals[2] = (*new_var) + 1;
        (*current_clause)++;
        (*new_var)++;
    }

    output->clauses[*current_clause].literals[0] = -(*new_var);
    output->clauses[*current_clause].literals[1] = input->literals[k-2];
    output->clauses[*current_clause].literals[2] = input->literals[k-1];
    (*current_clause)++;
    (*new_var)++;
}

// Fonction de réduction principale
Formula3SAT* reduce_sat_to_3sat_linear(Formula* sat_formula) {
    int max_clauses = 0;
    for(int i = 0; i < sat_formula->num_clauses; i++) {
        int k = sat_formula->clauses[i].num_literals;
        if(k == 1) max_clauses += 4;
        else if(k == 2) max_clauses += 2;
        else if(k == 3) max_clauses += 1;
        else max_clauses += (k-2);
    }

    Formula3SAT* result = (Formula3SAT*)malloc(sizeof(Formula3SAT));
    if (result == NULL) {
        printf("Erreur d'allocation mémoire pour le résultat\n");
        exit(1);
    }
    result->clauses = (Clause3SAT*)malloc(max_clauses * sizeof(Clause3SAT));
    if (result->clauses == NULL) {
        printf("Erreur d'allocation mémoire pour les clauses\n");
        free(result);
        exit(1);
    }

    int current_clause = 0;
    int new_var = sat_formula->num_vars + 1;

    // [Reste de la fonction reduce_sat_to_3sat_linear inchangé]
    // ... [votre code existant]

    return result;
}

// Fonction pour calculer la complexité spatiale
size_t calculate_spatial_complexity(Formula* sat_formula, Formula3SAT* result) {
    size_t original_size = sizeof(Formula) +
                          (sat_formula->num_clauses * sizeof(Clause));

    size_t result_size = sizeof(Formula3SAT) +
                        (result->num_clauses * sizeof(Clause3SAT));

    return original_size + result_size;
}

void write_test_results(const char* filename, int test_num, int num_vars, int num_clauses,
                       int original_clauses, int result_clauses,
                       int new_vars, Complexity complexity) {
    FILE* file = fopen(filename, "a");
    if(file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return;
    }

    fprintf(file, "%d,%d,%d,%d,%d,%d,%.6f,%zu,%zu,%zu\n",
            test_num, num_vars, num_clauses, original_clauses,
            result_clauses, new_vars, complexity.time_taken,
            complexity.original_memory, complexity.result_memory,
            complexity.total_memory);
    fclose(file);
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
void run_test_cases(int num_tests) {
    int k=1000; 
    printf("Début des tests...\n");

    FILE* file = fopen("sat_reduction_results.csv", "w");
    if (file == NULL) {
        printf("Erreur lors de la création du fichier CSV\n");
        return;
    }
    fprintf(file, "Test_Num,Variables_Originales,Clauses_Originales,Clauses_Initial," 
            "Clauses_3SAT,Nouvelles_Variables,Temps_Execution,"
            "Memoire_Originale(bytes),Memoire_Resultat(bytes),Memoire_Totale(bytes)\n");
    fclose(file);

    for(int test = 0; test < num_tests; test++) {
        printf("\n=== Test %d/%d ===\n", test + 1, num_tests);

        // Paramètres augmentés pour des tests plus significatifs
        int num_vars = 10 + rand() % 91;     // Entre 10 et 100 variables
        int num_clauses = 20 + rand() % 81;  // Entre 20 et 100 clauses
        int max_literals = 5 + rand() % 16;  // Entre 5 et 20 littéraux par clause

        printf("Paramètres: %d variables, %d clauses, max %d littéraux par clause\n",
               num_vars, num_clauses, max_literals);

        Formula* sat_formula = generate_random_sat(num_vars, num_clauses, max_literals);

        // Mesure du temps
        clock_t start, end;
        start = clock(); // Démarre le chronomètre

        Formula3SAT* result = reduce_sat_to_3sat_linear(sat_formula);

        end = clock(); // Arrête le chronomètre

        // Calcul des complexités
        Complexity complexity;
        complexity.time_taken = complexite(k,end,start);

        complexity.original_memory = sizeof(Formula) +
                                   (sat_formula->num_clauses * sizeof(Clause));
        complexity.result_memory = sizeof(Formula3SAT) +
                                 (result->num_clauses * sizeof(Clause3SAT));
        complexity.total_memory = complexity.original_memory + complexity.result_memory;

        // Affichage des résultats
        printf("Résultat: %d clauses 3-SAT, %d nouvelles variables\n",
               result->num_clauses, result->num_vars - sat_formula->num_vars);
        printf("Temps d'exécution: %.9f secondes\n", complexity.time_taken);
        printf("Complexité spatiale:\n");
        printf("  - Mémoire originale: %zu bytes\n", complexity.original_memory);
        printf("  - Mémoire résultat: %zu bytes\n", complexity.result_memory);
        printf("  - Mémoire totale: %zu bytes\n", complexity.total_memory);

        write_test_results("sat_reduction_results.csv",
                          test + 1,
                          num_vars,
                          num_clauses,
                          sat_formula->num_clauses,
                          result->num_clauses,
                          result->num_vars - sat_formula->num_vars,
                          complexity);

        free(result->clauses);
        free(result);
        free(sat_formula);
    }
    printf("\nTests terminés. Résultats enregistrés dans sat_reduction_results.csv\n");
}


int main() {
    srand(time(NULL));
    int num_tests = 20; // Réduit à 10 tests pour la démonstration
    run_test_cases(num_tests);
    return 0;
}
