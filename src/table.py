from tabulate import tabulate

# Dati per le tabelle
mpi_data = [
    ["Size", "Speedup"],
    [10000, 0.6615],
    [20000, 0.6863],
    [30000, 4.1096]
]

cuda_data = [
    ["Size", "Speedup"],
    [10000, 0.9778],
    [20000, 0.9864],
    [30000, 0.9925]
]

cuda_mpi_data = [
    ["Size", "Speedup"],
    [10000, 3.9050],
    [20000, 4.7514],
    [30000, 3.8701]
]

# Generazione delle tabelle
mpi_table = tabulate(mpi_data, headers="firstrow", tablefmt="pipe")
cuda_table = tabulate(cuda_data, headers="firstrow", tablefmt="pipe")
cuda_mpi_table = tabulate(cuda_mpi_data, headers="firstrow", tablefmt="pipe")

# Stampa delle tabelle
print("MPI:")
print(mpi_table)
print("\nCUDA:")
print(cuda_table)
print("\nCUDA+MPI:")
print(cuda_mpi_table)