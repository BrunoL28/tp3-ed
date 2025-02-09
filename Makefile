# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Iinclude

# Diretórios de origem e destino dos arquivos
OBJDIR = obj
BINDIR = bin
INPUTSDIR = inputs
BENCHMARKSDIR = benchmarks
GRAPHSDIR = graphs

# Nomes dos executáveis
TARGET = busca_voos.out
BENCHMARK_TARGET = benchmark.out

# Fontes principais e do benchmark
SRCS = src/main.cpp src/DateTime.cpp
BENCHMARK_SRCS = src/Benchmark.cpp src/DateTime.cpp

# Objetos
OBJS = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(SRCS))
BENCHMARK_OBJS = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(BENCHMARK_SRCS))

# Tamanhos dos arquivos de entrada
SIZES = 100 1000 5000 10000 50000 100000 250000 500000

# Alvo padrão: compila tudo
all: $(BINDIR)/$(TARGET) $(BINDIR)/$(BENCHMARK_TARGET)

# Compila o executável principal
$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$(TARGET) $(OBJS)

# Compila o benchmark separadamente
$(BINDIR)/$(BENCHMARK_TARGET): $(BENCHMARK_OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$(BENCHMARK_TARGET) $(BENCHMARK_OBJS)

# Regra para compilar os .cpp em .o, colocando os objetos na pasta obj
$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para gerar os arquivos de entrada automaticamente antes dos benchmarks
generate_inputs:
	@echo "📂 Gerando entradas para os benchmarks..."
	@mkdir -p $(INPUTSDIR)
	python3 ./python/generate_inputs.py

# Regra para rodar os benchmarks para todos os tamanhos definidos
benchmark: $(BINDIR)/$(BENCHMARK_TARGET)
	@mkdir -p $(BENCHMARKSDIR)
	@for size in $(SIZES); do \
		echo "🚀 Executando benchmark para flights_$$size.txt..."; \
		./$(BINDIR)/$(BENCHMARK_TARGET) $(INPUTSDIR)/flights_$$size.txt > $(BENCHMARKSDIR)/benchmark_$$size.txt; \
	done
	@echo "✅ Benchmark concluído. Resultados em $(BENCHMARKSDIR)/"

# Regra para gerar gráficos após a execução do benchmark
generate_graphs:
	@echo "📊 Gerando gráficos a partir dos resultados do benchmark..."
	@mkdir -p $(GRAPHSDIR)
	python3 ./python/generate_graphs.py

# Regra para limpar diretórios de objetos, executáveis e arquivos temporários
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(INPUTSDIR) $(BENCHMARKSDIR) $(GRAPHSDIR)

# Regra para rodar toda a análise experimental automaticamente
full_pipeline: generate_inputs benchmark generate_graphs
	@echo "🚀 Pipeline completo executado com sucesso!"