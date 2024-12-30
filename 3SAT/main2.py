import pandas as pd
import matplotlib.pyplot as plt

# Charger les données à partir du fichier CSV
filename = "3SAT//resultat_ver_sol.csv"
data = pd.read_csv(filename)

# Graphique du temps moyen en fonction du nombre de clauses
plt.figure(figsize=(10, 6))
plt.plot(data['nbr_clauses'], data['temps'], label='Temps Moyen (s)', color='b', marker='o')
plt.xlabel("Nombre de clauses")
plt.ylabel("Temps Moyen (s)")
plt.title("Temps moyen en fonction du nombre de clauses")
plt.legend()
plt.grid(True)
plt.show()

# Graphique de la mémoire en fonction du nombre de clauses
plt.figure(figsize=(10, 6))
plt.plot(data['nbr_clauses'], data['memUsage'], label='Mémoire utilisée (bytes)', color='g', marker='x')
plt.xlabel("Nombre de clauses")
plt.ylabel("Mémoire utilisée (bytes)")
plt.title("Mémoire utilisée en fonction du nombre de clauses")
plt.legend()
plt.grid(True)
plt.show()
