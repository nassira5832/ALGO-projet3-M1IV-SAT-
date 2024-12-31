import pandas as pd
import matplotlib.pyplot as plt

# Charger les données à partir du fichier CSV généré par le code C
filename = "SAT//resultatSAT_trv_Sol.csv"
data = pd.read_csv(filename)

# Graphique du temps en fonction du nombre de clauses
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


import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Charger les données à partir du fichier CSV généré par le code C
filename = "resultatSAT_trv_Sol.csv"
data = pd.read_csv(filename)

# Graphique en 3D du temps en fonction de nbr_clauses, num_vars et num_literals
fig = plt.figure(figsize=(10, 6))
ax = fig.add_subplot(111, projection='3d')

# Définir les axes
x = data['nbr_clauses']  # Nombre de clauses
y = data['num_vars']     # Nombre de variables
z = data['num_literals'] # Nombre de littéraux
time = data['temps']     # Temps en fonction des autres paramètres

# Tracer le graphique
ax.scatter(x, y, z, c=time, cmap='viridis', marker='o')
ax.set_xlabel('Nombre de clauses')
ax.set_ylabel('Nombre de variables')
ax.set_zlabel('Nombre de littéraux')
ax.set_title('Temps en fonction de nbr_clauses, num_vars et num_literals')

# Afficher la légende
fig.colorbar(ax.scatter(x, y, z, c=time, cmap='viridis', marker='o'), ax=ax, label='Temps (s)')
plt.show()

# Graphique en 3D de la mémoire en fonction de nbr_clauses, num_vars et num_literals
fig = plt.figure(figsize=(10, 6))
ax = fig.add_subplot(111, projection='3d')

# Mémoire en fonction des autres paramètres
memory = data['memUsage']

# Tracer le graphique
ax.scatter(x, y, z, c=memory, cmap='plasma', marker='x')
ax.set_xlabel('Nombre de clauses')
ax.set_ylabel('Nombre de variables')
ax.set_zlabel('Nombre de littéraux')
ax.set_title('Mémoire en fonction de nbr_clauses, num_vars et num_literals')

# Afficher la légende
fig.colorbar(ax.scatter(x, y, z, c=memory, cmap='plasma', marker='x'), ax=ax, label='Mémoire utilisée (bytes)')
plt.show()
