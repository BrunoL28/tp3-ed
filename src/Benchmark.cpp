#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include "../include/AVLTree.hpp"
#include "../include/Flight.hpp"
#include "../include/Sort.hpp"

using namespace std;
using namespace std::chrono;

int compareDoubles(const double &a, const double &b) {
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

struct FlightData {
    string origin;
    string destination;
    double price;
    int seats;
    string departure;
    string arrival;
    int stops;
};

vector<FlightData> loadFlights(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erro ao abrir " << filename << endl;
        exit(1);
    }

    int flightCount;
    file >> flightCount;
    if (flightCount <= 0) {
        cerr << "Erro: número inválido de voos." << endl;
        exit(1);
    }

    vector<FlightData> flights(flightCount);
    for (int i = 0; i < flightCount; i++) {
        file >> flights[i].origin >> flights[i].destination >> flights[i].price
             >> flights[i].seats >> flights[i].departure >> flights[i].arrival
             >> flights[i].stops;
    }

    return flights;
}

void runBenchmarks(const string &filename, int size) {
    vector<FlightData> flights = loadFlights(filename);
    if (flights.size() < size) return;

    vector<FlightData> sampleFlights(flights.begin(), flights.begin() + size);

    // Medindo inserção AVL
    AVLTree<double> avlTree(compareDoubles);
    auto start_insert_avl = high_resolution_clock::now();
    for (auto &flight : sampleFlights) {
        avlTree.insert(flight.price, nullptr);
    }
    auto end_insert_avl = high_resolution_clock::now();
    double insertTimeAVL = duration<double, milli>(end_insert_avl - start_insert_avl).count();

    // Medindo inserção linear
    vector<double> linearFlights;
    auto start_insert_linear = high_resolution_clock::now();
    for (auto &flight : sampleFlights) {
        linearFlights.push_back(flight.price);
    }
    auto end_insert_linear = high_resolution_clock::now();
    double insertTimeLinear = duration<double, milli>(end_insert_linear - start_insert_linear).count();

    // Medindo ordenação
    auto start_sorting = high_resolution_clock::now();
    sort(linearFlights.begin(), linearFlights.end());
    auto end_sorting = high_resolution_clock::now();
    double sortingTime = duration<double, milli>(end_sorting - start_sorting).count();

    ofstream output("benchmarks/benchmark_" + to_string(size) + ".txt");
    output << "Tamanho\tInserçãoAVL(ms)\tInserçãoLinear(ms)\tOrdenação(ms)\n";
    output << size << "\t" << insertTimeAVL << "\t" << insertTimeLinear << "\t" << sortingTime << "\n";
}

int main() {
    vector<int> sizes = {100, 1000, 5000, 10000, 50000, 100000, 250000, 500000};
    for (int size : sizes) {
        string filename = "inputs/flights_" + to_string(size) + ".txt";
        cout << "Rodando benchmark para " << size << " voos..." << endl;
        runBenchmarks(filename, size);
    }
    return 0;
}