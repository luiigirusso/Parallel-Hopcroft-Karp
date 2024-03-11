import pandas as pd
import matplotlib.pyplot as plt

# Carica i dati dai file CSV
df_sequential = pd.read_csv('../measures/output_sequential.csv')
df_mpi = pd.read_csv('../measures/output_mpi.csv')
df_cuda = pd.read_csv('../measures/output_cuda.csv')
df_cuda_mpi = pd.read_csv('../measures/output_cuda_mpi.csv')

# Crea una lista dei DataFrame per ogni dimensione
dfs = [df_sequential, df_mpi, df_cuda, df_cuda_mpi]

# Definisci dimensioni e nomi dei programmi
dimensioni = [10000, 20000, 30000]
nomi_programmi = ['Sequential', 'MPI', 'CUDA', 'CUDA + MPI']
colori = ['blue', 'green', 'red', 'orange']

# Crea tre grafici separati per le tre dimensioni
for dimensione in dimensioni:
    plt.figure(figsize=(10, 6))
    plt.title(f'Comparison of configurations at size {dimensione}')
    plt.xlabel('Configuration')
    plt.ylabel('Execution time (s)')
    plt.grid(True)

    # Estrai i dati per la dimensione corrente
    dati_dimensione = [df[df['Dimensione'] == dimensione] for df in dfs]

    # Plotta i dati per ogni programma
    for i, df in enumerate(dati_dimensione):
        plt.bar(nomi_programmi[i], df['Tempo'], color=colori[i], label=nomi_programmi[i])

    plt.legend()
    plt.show()
