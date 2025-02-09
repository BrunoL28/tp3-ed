#!/usr/bin/env python3
import random
from datetime import datetime, timedelta

def random_date(start, end):
    delta = end - start
    random_seconds = random.randint(0, int(delta.total_seconds()))
    return start + timedelta(seconds=random_seconds)

def format_date(dt):
    return dt.strftime("%Y-%m-%dT%H:%M:%S")

def generate_voos(num):
    # Códigos de exemplo para origem e destino
    codes = ["ATL", "BOS", "LAX", "JFK", "ORD", "SFO", "MIA", "DFW", "DEN", "CLT"]
    voos = []
    start_date = datetime(2025, 1, 1)
    end_date = datetime(2025, 12, 31)
    for i in range(num):
        org = random.choice(codes)
        dst = random.choice(codes)
        while dst == org:
            dst = random.choice(codes)
        preco = round(random.uniform(50, 1500), 2)
        seats = random.randint(0, 100)
        dep = random_date(start_date, end_date)
        # Garantir que a duração seja entre 1h e 10h
        dur = timedelta(hours=random.randint(1, 10))
        arr = dep + dur
        stops = random.randint(0, 3)
        linha = f"{org} {dst} {preco} {seats} {format_date(dep)} {format_date(arr)} {stops}"
        voos.append(linha)
    return voos

def generate_consultas():
    # Vamos criar 3 consultas exemplares, incluindo operadores OR e NOT.
    consultas = []
    # Consulta 1: Testa OR (||)
    # Exemplo: Retorne 4 voos onde (preço >= 600) OR (duração <= 5000)
    consultas.append("4 pds (((prc>=600)||(dur<=5000)))")
    # Consulta 2: Testa NOT (!)
    # Exemplo: Retorne 2 voos onde a origem NÃO seja BOS e que tenham 0 paradas.
    consultas.append("2 dps (((! (org==BOS))&&(sto==0)))")
    # Consulta 3: Combinação de OR e NOT
    # Exemplo: Retorne 3 voos onde ((preço <= 800) OR (não (dest==JFK))) e (seats>=10)
    consultas.append("3 pds (((prc<=800)||( ! (dst==JFK) ))&&(sea>=10))")
    return consultas

if __name__ == "__main__":
    num_voos = 10
    voos = generate_voos(num_voos)
    consultas = generate_consultas()
    
    with open("input_test.txt", "w") as f:
        f.write(f"{num_voos}\n")
        for v in voos:
            f.write(v + "\n")
        f.write(f"{len(consultas)}\n")
        for c in consultas:
            f.write(c + "\n")
