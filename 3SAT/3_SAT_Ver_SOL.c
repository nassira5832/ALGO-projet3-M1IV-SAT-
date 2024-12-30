#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    bool x1, x2, x3;
} clause;

typedef struct {
    int nbr_clauses;
    clause *clauses;
} formule;

clause generer_clause() {
    clause c;
    c.x1 = rand() % 2; 
    c.x2 = rand() % 2; 
    c.x3 = rand() % 2;
    return c;
}

formule generer_Formule(int nbr_clauses) {
    formule F;
    F.nbr_clauses = nbr_clauses;
    F.clauses = (clause *)malloc(nbr_clauses * sizeof(clause)); // Allocation dynamique de mémoire
    for (int i = 0; i < nbr_clauses; i++) {
        F.clauses[i] = generer_clause();
    }
    return F;
}

bool evaluer_clause(clause c, bool x1, bool x2, bool x3) {
    return (c.x1 && x1) || (c.x2 && x2) || (c.x3 && x3);
}

bool verefier_sol (formule F , clause c ){
    for (int i=0 ; i<F.nbr_clauses ; i++){
        if(!evaluer_clause(F.clauses[i],c.x1 ,c.x2 ,c.x3)){
            return false;
        }
    }
    return true;
}
double complexite (int k  , double t2 , double t1 ){
    double time=0; 
    double T;
    for (int i=1 ; i<=k ; i++){
       time= time+(t2 - t1)/CLOCKS_PER_SEC;
    }
    T=time/k;
    return T ; 
}
int main() {
    int start =2; 
    int end = 50000; 
    int step =2 ; 
    int k=3000;
    FILE *file=fopen("resultat_ver_sol.csv","w");
    fprintf(file, "nbr_clauses,temps,memUsage\n");
    while(start<end){
     double t1= clock();
     formule F = generer_Formule(start);
     clause solution = generer_clause();
     verefier_sol(F, solution);
     double t2= clock();
     double temps=complexite(k,t2,t1);
     size_t memoire = sizeof(F) + (F.nbr_clauses * sizeof(clause));
     fprintf(file,"%d,%f,%zu\n",start,temps , memoire);
     start=start+step;
    }
    
    return 0 ; 
}
 
