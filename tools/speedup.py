import csv

def read_data(filename):
    data = {}
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # Salta l'intestazione
        for row in reader:
            size = int(row[0])
            time = float(row[1])
            if size in data:
                data[size].append(time)
            else:
                data[size] = [time]
    return data

def calculate_speedup(serial_times, parallel_times):
    speedup = {}
    for size in serial_times:
        serial_time = sum(serial_times[size]) / len(serial_times[size])
        parallel_time = sum(parallel_times[size]) / len(parallel_times[size])
        speedup[size] = serial_time / parallel_time
    return speedup

def main():
    serial_data = read_data("../measures/output_sequential.csv")
    parallel_data = read_data("../measures/output_cuda_mpi.csv")

    speedup = calculate_speedup(serial_data, parallel_data)

    print("Speedup:")
    for size in speedup:
        print("Size:", size, "Speedup:", speedup[size])

if __name__ == "__main__":
    main()
