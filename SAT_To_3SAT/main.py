import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def load_data(file_path):
    """Charger les données du fichier CSV et afficher les colonnes disponibles"""
    data = pd.read_csv(file_path)
    print("Colonnes disponibles dans le CSV:", data.columns)  # Afficher les colonnes pour vérification
    return data

def create_space_complexity_plot(data):
    """Créer le graphique de complexité spatiale"""
    plt.figure(figsize=(12, 6))

    # Créer un scatter plot avec une ligne de tendance
    plt.scatter(data['Variables_Originales'], data['Memoire_Originale'],  # Vérifier que 'Memoire_Originale' existe bien dans le fichier CSV
                alpha=0.5, label='Mémoire originale')

    # Ajouter une ligne de tendance
    z = np.polyfit(data['Variables_Originales'], data['Memoire_Originale'], 1)
    p = np.poly1d(z)
    plt.plot(data['Variables_Originales'], p(data['Variables_Originales']),
             "r--", alpha=0.8, label='Ligne de tendance')

    plt.xlabel('Nombre de variables')
    plt.ylabel('Mémoire utilisée (bytes)')
    plt.title('Complexité spatiale de la réduction SAT vers 3-SAT')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.savefig('space_complexity.png')
    plt.close()

# Exemple d'appel de la fonction avec un chemin vers un fichier CSV
file_path = 'SAT_To_3SAT\\sat_reduction_results.csv'  
data = load_data(file_path)

# Vous pouvez renommer les colonnes si nécessaire
# Exemple de renommer les colonnes si elles ne sont pas correctement nommées dans le fichier
data.columns = ['Test_Num', 'Variables_Originales', 'Clauses_Originales', 'Clauses_Initial', 
                'Clauses_3SAT', 'Nouvelles_Variables', 'Temps_Execution', 
                'Memoire_Originale', 'Memoire_Resultat', 'Memoire_Totale']

# Appeler la fonction pour créer le graphique
create_space_complexity_plot(data)
