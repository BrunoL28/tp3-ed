import os
import random
import datetime

# Lista de códigos de aeroportos simulados
AIRPORT_CODES = ["ATL", "BOS", "CLT", "DFW", "DEN", "DTW", "EWR", "IAD",
                 "JFK", "LAX", "LGA", "MIA", "OAK", "ORD", "PHL", "SFO"]

# Tamanhos das entradas
SIZES = [100, 1000, 5000, 10000, 50000, 100000, 250000, 500000]

def random_datetime():
    """Gera uma data e hora aleatória entre 2022-01-01 e 2024-12-31."""
    start_date = datetime.datetime(2022, 1, 1)
    end_date = datetime.datetime(2024, 12, 31)
    random_days = random.randint(0, (end_date - start_date).days)
    random_seconds = random.randint(0, 24 * 3600 - 1)
    dt = start_date + datetime.timedelta(days=random_days, seconds=random_seconds)
    return dt.strftime("%Y-%m-%dT%H:%M:%S")

def generate_flight_data(n_flights, filename):
    """Gera um arquivo de entrada com voos aleatórios."""
    with open(filename, "w") as f:
        f.write(f"{n_flights}\n")
        
        for _ in range(n_flights):
            origin = random.choice(AIRPORT_CODES)
            destination = random.choice(AIRPORT_CODES)
            while destination == origin:
                destination = random.choice(AIRPORT_CODES)

            price = round(random.uniform(50, 1500), 2)
            seats = random.randint(1, 10)
            dep_time = random_datetime()
            arr_time = random_datetime()
            while arr_time <= dep_time:
                arr_time = random_datetime()

            stops = random.randint(0, 3)
            f.write(f"{origin} {destination} {price} {seats} {dep_time} {arr_time} {stops}\n")

if __name__ == "__main__":
    os.makedirs("inputs", exist_ok=True)
    for size in SIZES:
        filename = f"inputs/flights_{size}.txt"
        print(f"Gerando entrada: {filename} com {size} voos...")
        generate_flight_data(size, filename)