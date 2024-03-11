#!/bin/bash

make clean
make

# Cancella tutti i file nella cartella test
rm -f ./measures/*

# Numero di volte da eseguire il comando
numero_test=10

# Funzione per eseguire i test e aggiungere i risultati al file di testo
esegui_test() {
    comando=$1
    dimensione=$2
    output_file=$3

    { time $comando $dimensione $dimensione; } 2>> "$output_file"
}

# Funzione per eseguire i test e scrivere i risultati in un file CSV
scrivi_csv() {
    comando=$1
    tipo_test=$2
    dimensione=$3
    numero_test=$4

    csv_file="./measures/output_${tipo_test}.csv"

    if [ ! -f $csv_file ]; then
        echo "Dimensione,Tempo" > $csv_file
    fi

    for ((i=1; i<=$numero_test; i++)); do
        output_file="./measures/output_${tipo_test}_${dimensione}_${i}.txt"
        tempo=$( { time $comando $dimensione $dimensione; } 2>&1 | grep real | awk '{print $2}' )
        echo "$dimensione,$tempo" >> $csv_file
    done
}

# Esegui test per tipo (sequential, cuda, mpi, cuda_mpi)
tipi_test=("sequential" "cuda" "mpi" "cuda_mpi")

for tipo_test in "${tipi_test[@]}"; do
    for dimensione in 10000 20000 30000; do
        case $tipo_test in
            "sequential")
                comando="./bin/sequential";;
            "cuda")
                comando="./bin/cuda";;
            "mpi")
                comando="mpirun -np 4 ./bin/mpi";;
            "cuda_mpi")
                comando="mpirun -np 4 ./bin/cuda_mpi";;
        esac

        scrivi_csv "$comando" $tipo_test $dimensione $numero_test
    done
done
