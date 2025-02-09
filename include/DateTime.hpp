#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <ctime>

/**
 * @brief Converte uma string de data/hora (formato "YYYY-MM-DDTHH:MM:SS") para time_t em UTC.
 *        Milissegundos são ignorados.
 * 
 * @param dateTimeStr String de data/hora.
 * @return time_t Tempo correspondente em UTC.
 */
time_t parseDateTime(const char* dateTimeStr);

#endif // DATETIME_HPP