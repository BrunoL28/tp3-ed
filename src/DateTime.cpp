#include "../include/DateTime.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>

/**
 * @brief Converte uma string de data/hora (formato "YYYY-MM-DDTHH:MM:SS") para time_t em UTC.
 *        Milissegundos são ignorados.
 *
 * @param dateTimeStr String de data/hora.
 * @return time_t Tempo em UTC.
 */
time_t parseDateTime(const char* dateTimeStr) {
    int year, month, day, hour, minute, second;
    sscanf(dateTimeStr, "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    
    struct tm timeStruct;
    timeStruct.tm_year = year - 1900;
    timeStruct.tm_mon  = month - 1;
    timeStruct.tm_mday = day;
    timeStruct.tm_hour = hour;
    timeStruct.tm_min  = minute;
    timeStruct.tm_sec  = second;
    timeStruct.tm_isdst = 0;
    
    return timegm(&timeStruct);
}