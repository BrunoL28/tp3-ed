# Sistema de Consulta de Voos ✈️

Neste trabalho prático, desenvolvemos um sistema capaz de filtrar e ordenar dados de voos de forma eficiente, com base em critérios personalizados como preço, duração e número de paradas. Este sistema utiliza estruturas de dados avançadas como Árvores AVL e algoritmos como o Quicksort para garantir alto desempenho e precisão.

---

## 📖 Índice
1. [Introdução](#introdução)
2. [Funcionalidades Implementadas](#funcionalidades-implementadas)
3. [Estrutura do Projeto](#estrutura-do-projeto)
4. [Estruturas de Dados e Complexidade](#estruturas-de-dados-e-complexidade)
5. [Análise Experimental](#análise-experimental)
6. [Como Usar](#como-usar)
7. [Comandos do Makefile](#comandos-do-makefile)
8. [Contribuindo](#contribuindo)

---

## 🛫 Introdução

Este projeto simula uma aplicação real de consulta e ordenação de dados de voos para melhorar a experiência do usuário. Os usuários podem executar consultas complexas para filtrar voos com base em diversos parâmetros, como preço, horário de partida e número de paradas, utilizando operadores lógicos como AND, OR e NOT.

O sistema é modular e escalável, sendo capaz de lidar com datasets de milhões de registros.

---

## 🚀 Funcionalidades Implementadas

1. **Árvores AVL**:
   - Indexação eficiente dos voos por atributos como preço, duração e número de paradas.
   - Suporte a operações de inserção e consulta com complexidade média de \(O(\log n)\).

2. **Parser de Expressões**:
   - Interpreta consultas de usuários e transforma em árvores de expressões lógicas.
   - Suporte a operações lógicas complexas, como `(preço <= 500) OR (duração >= 8000)`.

3. **Quicksort**:
   - Ordenação dos resultados filtrados com base em critérios definidos pelo usuário.
   - Complexidade média \(O(n \log n)\).

4. **Benchmarking**:
   - Mede o desempenho de operações como inserção em AVL, inserção linear e Quicksort com grandes volumes de dados.
   - Gera relatórios de desempenho para análise.

5. **Integração com Python**:
   - Scripts para gerar datasets de teste e comparar saídas com os resultados esperados.
   - Geração de gráficos de desempenho.

---

## 📂 Estrutura do Projeto

- **`src/`**: Contém o código-fonte do projeto (arquivos `.cpp`).
- **`include/`**: Contém os arquivos de cabeçalho (`.hpp`).
- **`bin/`**: Contém o executável gerado pela compilação (`tp3.out`).
- **`obj/`**: Contém os arquivos objeto (`.o`) gerados durante a compilação.
- **`input/`**: Pequenos arquivos de entrada para testes.
- **`inputs/`**: Arquivos de entrada grandes, gerados para testar o sistema com altas cargas.
- **`outputs/`**: Saídas geradas pelo sistema após o processamento das entradas.
- **`expected_outputs/`**: Saídas esperadas para validação do sistema.
- **`python/`**:
  - Scripts para geração de entradas grandes e comparação de saídas.
  - Scripts para gerar gráficos de desempenho.
- **`graphs/`**: Contém gráficos de análise de desempenho gerados pelos scripts Python.

---

## 🏗️ Estruturas de Dados e Complexidade

### **Árvore AVL**
- **Propósito**: Indexar dados de voos por atributos como preço, duração e paradas.
- **Inserção**: \(O(\log n)\)
- **Consultas de intervalo**: \(O(\log n + k)\), onde \(k\) é o número de elementos encontrados.

### **Árvore de Expressões**
- **Propósito**: Processar consultas lógicas dos usuários.
- **Parsing**: \(O(m)\), onde \(m\) é o tamanho da string da consulta.
- **Avaliação**: Avaliação eficiente para campos indexados. Quando índices não são usados, recorre a uma varredura completa dos dados.

### **Quicksort**
- **Propósito**: Ordenar resultados de voos com base nos critérios especificados pelo usuário.
- **Complexidade**:
  - Média: \(O(n \log n)\)
  - Pior Caso: \(O(n^2)\) (raro devido à seleção de pivô).

---

## 📊 Análise Experimental

### **1. Desempenho da Árvore AVL vs. Inserção Linear**
- **Observação**: A inserção em Árvore AVL é significativamente mais rápida que a inserção linear à medida que o dataset cresce.
- **Motivo**: A árvore AVL mantém o balanceamento, garantindo complexidade logarítmica.

### **2. Desempenho do Quicksort**
- **Observação**: O Quicksort escala eficientemente com o aumento do tamanho dos dados.
- **Motivo**: A abordagem de divisão e conquista mantém a complexidade logarítmica na maioria dos casos.

Os gráficos que demonstram essas análises estão disponíveis na pasta `/graphs`.

---

## 🛠️ Como Usar

### **Formato dos Arquivos de Entrada**
Os arquivos de entrada seguem o seguinte formato:
```
<number_of_flights>
<origin> <destination> <price> <seats> <departure_time> <arrival_time> <stops>
...
<number_of_queries>
<max_results> <sort_criteria> <query_expression>
```
Exemplo:
```
10
JFK BOS 200.00 50 2025-02-10T10:00:00 2025-02-10T12:00:00 0
BOS JFK 300.00 60 2025-02-11T14:00:00 2025-02-11T16:00:00 1
...
2
5 price (preço<=300)
3 duration (paradas==0)
```

### **Compilando e Executando**
1. **Compilar o projeto**:
   ```bash
   make
   ```
2. **Executar o programa**:
   ```bash
   ./bin/tp3.out input/<arquivo_de_entrada>.txt
   ```
3. **Comparar saídas**:
   - Use o script Python na pasta `/python` para comparar as saídas geradas com os resultados esperados.

---

## 🔧 Comandos do Makefile

| Comando        | Descrição                                               |
|----------------|---------------------------------------------------------|
| `make`         | Compila o projeto.                                      |
| `make run`     | Executa o arquivo de entrada padrão na pasta `/input`.  |
| `make clean`   | Remove os arquivos de compilação gerados (`bin/`, `obj/`)|

---

## 🧪 Contribuindo

Contribuições são bem-vindas! Sinta-se à vontade para:
- Adicionar novas funcionalidades.
- Melhorar a documentação.
- Relatar bugs.

---

## 📈 Insights de Desempenho

- **Árvores AVL**: Excelentes para escalabilidade devido à complexidade logarítmica.
- **Quicksort**: Mantém eficiência mesmo com grandes volumes de dados, essencial para ordenação dinâmica.

Caso tenha dúvidas ou sugestões, entre em contato. Vamos construir um sistema ainda mais rápido e eficiente! 🚀
