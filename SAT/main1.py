import pandas as pd
import matplotlib.pyplot as plt

# Charger les données à partir du fichier CSV généré par le code C
filename = "SAT//resultatSAT_trv_Sol.csv"
data = pd.read_csv(filename)

# Trier les données par le nombre de clauses pour chaque graphique
data_sorted_by_clauses = data.sort_values(by='nbr_clauses')
data_sorted_by_vars = data.sort_values(by='num_vars')
data_sorted_by_literals = data.sort_values(by='num_literals')

# Premier graphique : Temps moyen en fonction de différentes variables
plt.figure(figsize=(12, 8))

# Tracer le temps moyen en fonction du nombre de clauses
plt.plot(data_sorted_by_clauses['nbr_clauses'], data_sorted_by_clauses['temps'], label='Temps Moyen (s) en fonction du nombre de clauses', color='b', marker='o')

# Tracer le temps moyen en fonction du nombre de variables
plt.plot(data_sorted_by_vars['num_vars'], data_sorted_by_vars['temps'], label='Temps Moyen (s) en fonction du nombre de variables', color='g', marker='x')

# Tracer le temps moyen en fonction du nombre de littéraux
plt.plot(data_sorted_by_literals['num_literals'], data_sorted_by_literals['temps'], label='Temps Moyen (s) en fonction du nombre de littéraux', color='r', marker='s')

plt.xlabel("Nombre de Clauses / Variables / Littéraux")
plt.ylabel("Temps Moyen (s)")
plt.title("Temps Moyen en Fonction du Nombre de Clauses, de Variables et de Littéraux")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Deuxième graphique : Mémoire utilisée en fonction de différentes variables
plt.figure(figsize=(12, 8))

# Tracer la mémoire utilisée en fonction du nombre de clauses
plt.plot(data_sorted_by_clauses['nbr_clauses'], data_sorted_by_clauses['memUsage'], label='Mémoire Utilisée (octets) en fonction du nombre de clauses', color='c', marker='^')

# Tracer la mémoire utilisée en fonction du nombre de variables
plt.plot(data_sorted_by_vars['num_vars'], data_sorted_by_vars['memUsage'], label='Mémoire Utilisée (octets) en fonction du nombre de variables', color='m', marker='x')

# Tracer la mémoire utilisée en fonction du nombre de littéraux
plt.plot(data_sorted_by_literals['num_literals'], data_sorted_by_literals['memUsage'], label='Mémoire Utilisée (octets) en fonction du nombre de littéraux', color='y', marker='s')

plt.xlabel("Nombre de Clauses / Variables / Littéraux")
plt.ylabel("Mémoire Utilisée (octets)")
plt.title("Mémoire Utilisée en Fonction du Nombre de Clauses, de Variables et de Littéraux")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
