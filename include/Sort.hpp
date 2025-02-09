#ifndef SORT_HPP
#define SORT_HPP

#include "Flight.hpp"
#include <string>
#include <cstdlib>
#include <cstring>

using std::string;

/**
 * @brief Compara dois voos com base nos critérios de ordenação.
 *
 * Os critérios podem ser:
 * - 'p' para preço,
 * - 'd' para duração,
 * - 's' para paradas.
 *
 * @param flightA Ponteiro para o primeiro voo.
 * @param flightB Ponteiro para o segundo voo.
 * @param orderCriteria String com os critérios de ordenação.
 * @return -1 se flightA < flightB, 1 se flightA > flightB, 0 se forem iguais.
 */
int compareFlightByCriteria(const Flight* flightA, const Flight* flightB, const string &orderCriteria) {
    for (size_t i = 0; i < orderCriteria.size(); i++) {
        char criterion = orderCriteria[i];
        if (criterion == 'p') {
            if (flightA->price < flightB->price) return -1;
            if (flightA->price > flightB->price) return 1;
        } else if (criterion == 'd') {
            if (flightA->duration < flightB->duration) return -1;
            if (flightA->duration > flightB->duration) return 1;
        } else if (criterion == 's') {
            if (flightA->stops < flightB->stops) return -1;
            if (flightA->stops > flightB->stops) return 1;
        }
    }
    return 0;
}

/**
 * @brief Troca dois ponteiros para Flight.
 * @param a Primeiro ponteiro.
 * @param b Segundo ponteiro.
 */
void swapFlightPointers(Flight*& a, Flight*& b) {
    Flight* temp = a;
    a = b;
    b = temp;
}

/**
 * @brief Particiona um array de voos para o quicksort.
 * @param arr Array de ponteiros para Flight.
 * @param low Índice inicial.
 * @param high Índice final.
 * @param orderCriteria Critérios de ordenação.
 * @return Índice de partição.
 */
int partitionFlights(Flight** arr, int low, int high, const string &orderCriteria) {
    Flight* pivot = arr[high];
    int i = low;
    for (int j = low; j < high; j++) {
        if (compareFlightByCriteria(arr[j], pivot, orderCriteria) < 0) {
            swapFlightPointers(arr[i], arr[j]);
            i++;
        }
    }
    swapFlightPointers(arr[i], arr[high]);
    return i;
}

/**
 * @brief Ordena um array de voos usando quicksort.
 * @param arr Array de ponteiros para Flight.
 * @param low Índice inicial.
 * @param high Índice final.
 * @param orderCriteria Critérios de ordenação.
 */
void quickSortFlights(Flight** arr, int low, int high, const string &orderCriteria) {
    if (low < high) {
        int partitionIndex = partitionFlights(arr, low, high, orderCriteria);
        quickSortFlights(arr, low, partitionIndex - 1, orderCriteria);
        quickSortFlights(arr, partitionIndex + 1, high, orderCriteria);
    }
}

#endif // SORT_HPP