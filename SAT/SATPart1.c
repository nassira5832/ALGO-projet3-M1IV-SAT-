#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int *literals;        // Tableau des littéraux de la clause
    int num_literals;     // Nombre de littéraux dans la clause
} Clause;

typedef struct {
    Clause *clauses;      // Tableau des clauses
    int num_clauses;      // Nombre de clauses
    int num_vars;         // Nombre de variables
} Formula;

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

void read_formula(Formula *formula) {
    printf("Entrez le nombre de variables : ");
    scanf("%d", &formula->num_vars);

    printf("Entrez le nombre de clauses : ");
    scanf("%d", &formula->num_clauses);

    formula->clauses = malloc(formula->num_clauses * sizeof(Clause));

    for (int i = 0; i < formula->num_clauses; i++) {
        printf("Entrez le nombre de littéraux pour la clause %d : ", i + 1);
        scanf("%d", &formula->clauses[i].num_literals);

        formula->clauses[i].literals = malloc(formula->clauses[i].num_literals * sizeof(int));

        printf("Entrez les littéraux (nombre positif pour x, négatif pour ¬x) : ");
        for (int j = 0; j < formula->clauses[i].num_literals; j++) {
            scanf("%d", &formula->clauses[i].literals[j]);
        }
    }
}

void free_formula(Formula *formula) {
    for (int i = 0; i < formula->num_clauses; i++) {
        free(formula->clauses[i].literals); 
    }
    free(formula->clauses); 
}

// Fonction pour calculer le temps de complexité (en utilisant un simple calcul)
double complexite(int k, double t2, double t1) {
    return (t2 - t1) / CLOCKS_PER_SEC; // Conversion en secondes
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

int main() {
    srand(time(NULL)); // Initialisation du générateur aléatoire

    int start = 2;
    int end = 14;
    int Step = 2;
    int k = 20000;

    FILE *F = fopen("resultat3SAT_trv_Sol.csv", "w");
    if (F == NULL) {
        perror("Erreur d'ouverture du fichier");
        return 1;
    }
    fprintf(F, "nbr_clauses,num_vars,num_literals,temps,memUsage\n");

    while (start < end) {

        double t1 = clock(); // Début du chrono

        Formula formule = generer_Formule(start);
        bool *assignment = malloc(formule.num_vars * sizeof(bool));

        bool result = solve_sat(&formule, assignment);

        double t2 = clock(); // Fin du chrono
        double temps = complexite(k, t2, t1);

        size_t memoire = formule.num_clauses * sizeof(Clause) + formule.num_clauses * 3 * sizeof(int); // Estimation de la mémoire utilisée

        // Enregistrement des résultats dans le fichier CSV
        fprintf(F, "%d,%d,%d,%f,%zu\n", formule.num_clauses, formule.num_vars, formule.clauses[0].num_literals, temps, memoire);

        free(assignment);
        free_formula(&formule);

        start = start + Step;
    }

    fclose(F);
    return 0;
}
