import pandas as pd
import matplotlib.pyplot as plt

# Charger les données à partir du fichier CSV
filename = "3SAT_AVEC_SATLIB\\resultat3SAT_trv_Sol.csv"
data = pd.read_csv(filename)

# Vérifiez les noms des colonnes
print(data.columns)

# Trier les données par le nombre de clauses
data_sorted_by_clauses = data.sort_values(by='nbr_clauses')

# Premier graphique : Temps moyen en fonction du nombre de clauses
plt.figure(figsize=(12, 8))
plt.plot(data_sorted_by_clauses['nbr_clauses'], data_sorted_by_clauses['temps'], label='Temps Moyen (s) en fonction du nombre de clauses', color='b', marker='o')

plt.xlabel("Nombre de Clauses")
plt.ylabel("Temps Moyen (s)")
plt.title("Temps Moyen en Fonction du Nombre de Clauses")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Deuxième graphique : Mémoire utilisée en fonction du nombre de clauses
plt.figure(figsize=(12, 8))
plt.plot(data_sorted_by_clauses['nbr_clauses'], data_sorted_by_clauses['memUsage'], label='Mémoire Utilisée (octets) en fonction du nombre de clauses', color='c', marker='^')

plt.xlabel("Nombre de Clauses")
plt.ylabel("Mémoire Utilisée (octets)")
plt.title("Mémoire Utilisée en Fonction du Nombre de Clauses")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
