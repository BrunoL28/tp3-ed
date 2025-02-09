import os
import matplotlib.pyplot as plt
import numpy as np

RESULTS_DIR = "benchmarks"
GRAPHS_DIR = "graphs"
SIZES = [100, 1000, 5000, 10000, 50000, 100000, 250000, 500000]

def read_benchmark_results():
    """Lê os resultados dos benchmarks e retorna os dados formatados."""
    sizes, avl_insert, linear_insert, sorting = [], [], [], []

    for size in SIZES:
        filename = f"{RESULTS_DIR}/benchmark_{size}.txt"
        if not os.path.exists(filename):
            continue

        with open(filename, "r") as file:
            lines = file.readlines()[1:]  # Ignora cabeçalho
            for line in lines:
                values = line.strip().split("\t")
                if len(values) < 4:
                    continue

                sizes.append(int(values[0]))
                avl_insert.append(float(values[1]))
                linear_insert.append(float(values[2]))
                sorting.append(float(values[3]))

    return sizes, avl_insert, linear_insert, sorting

def plot_results(sizes, avl_insert, linear_insert, sorting):
    """Gera gráficos comparando os tempos médios de execução."""
    os.makedirs(GRAPHS_DIR, exist_ok=True)

    # Inserção AVL vs Inserção Linear
    plt.figure(figsize=(8, 5))
    plt.plot(sizes, avl_insert, label="Inserção AVL", marker="o")
    plt.plot(sizes, linear_insert, label="Inserção Linear", marker="o", linestyle="--")
    plt.xlabel("Número de Voos")
    plt.ylabel("Tempo (ms)")
    plt.title("Comparação: Inserção AVL vs Inserção Linear")
    plt.legend()
    plt.savefig(f"{GRAPHS_DIR}/insercao_comparacao.png")
    plt.close()

    # Ordenação
    plt.figure(figsize=(8, 5))
    plt.plot(sizes, sorting, label="Tempo de Ordenação", marker="o", color="red")
    plt.xlabel("Número de Voos")
    plt.ylabel("Tempo (ms)")
    plt.title("Tempo de Ordenação QuickSort")
    plt.legend()
    plt.savefig(f"{GRAPHS_DIR}/ordenacao.png")
    plt.close()

# Executa a análise
sizes, avl_insert, linear_insert, sorting = read_benchmark_results()
plot_results(sizes, avl_insert, linear_insert, sorting)