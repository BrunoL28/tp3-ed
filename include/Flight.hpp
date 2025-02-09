#ifndef FLIGHT_HPP
#define FLIGHT_HPP

#include <ctime>

/**
 * @brief Estrutura que representa um voo.
 */
struct Flight {
    char origin[4];         ///< Código da origem.
    char destination[4];    ///< Código do destino.
    double price;           ///< Preço do voo.
    int seats;              ///< Número de assentos disponíveis.
    char departureStr[40];  ///< Data/hora de partida (string original).
    char arrivalStr[40];    ///< Data/hora de chegada (string original).
    int stops;              ///< Número de paradas.

    // Campos calculados:
    time_t dep_time;        ///< Data/hora de partida (numérico).
    time_t arr_time;        ///< Data/hora de chegada (numérico).
    int duration;           ///< Duração em segundos (arr_time - dep_time).
};

#endif // FLIGHT_HPP