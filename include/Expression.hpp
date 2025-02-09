#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "Flight.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>

using std::string;

/**
 * @brief Classe abstrata para expressões.
 */
class Expr {
public:
    /**
     * @brief Avalia a expressão para um dado voo.
     * @param flight Objeto Flight a ser avaliado.
     * @return true se a expressão for satisfeita; false caso contrário.
     */
    virtual bool evaluate(const Flight &flight) = 0;

    virtual ~Expr() {}
};

/**
 * @brief Representa uma expressão lógica binária (AND, OR).
 */
class BinaryExpr : public Expr {
public:
    char op;     ///< '&' para AND, '|' para OR.
    Expr *left;  ///< Subexpressão à esquerda.
    Expr *right; ///< Subexpressão à direita.

    /**
     * @brief Construtor.
     */
    BinaryExpr() : op(0), left(nullptr), right(nullptr) {}

    /**
     * @brief Avalia a expressão binária.
     * @param flight Objeto Flight.
     * @return Resultado da operação lógica.
     */
    virtual bool evaluate(const Flight &flight) {
        if (op == '&')
            return left->evaluate(flight) && right->evaluate(flight);
        else if (op == '|')
            return left->evaluate(flight) || right->evaluate(flight);
        return false;
    }

    /**
     * @brief Destrutor.
     */
    virtual ~BinaryExpr() {
        delete left;
        delete right;
    }
};

/**
 * @brief Representa uma expressão lógica de negação (NOT).
 */
class NotExpr : public Expr {
public:
    Expr *child;  ///< Expressão a ser negada.

    /**
     * @brief Construtor.
     */
    NotExpr() : child(nullptr) {}

    /**
     * @brief Avalia a expressão NOT.
     * @param flight Objeto Flight.
     * @return Negação da avaliação da subexpressão.
     */
    virtual bool evaluate(const Flight &flight) {
        return !child->evaluate(flight);
    }

    /**
     * @brief Destrutor.
     */
    virtual ~NotExpr() {
        delete child;
    }
};

/**
 * @brief Representa um predicado de comparação para um campo de Flight.
 */
class PredicateExpr : public Expr {
public:
    /**
     * @brief Operadores de comparação.
     */
    enum CompOp { EQ, NE, LT, LE, GT, GE };

    string field;     ///< Nome do campo (ex.: "org", "dst", "prc", etc.).
    CompOp op;        ///< Operador de comparação.
    bool isNumeric;   ///< True se o campo for numérico.
    double numValue;  ///< Valor numérico para comparação (para campos como preço, duração, etc.).
    string strValue;  ///< Valor em string para comparação (para campos como origem, destino).

    /**
     * @brief Avalia o predicado para um dado voo.
     * @param flight Objeto Flight.
     * @return true se o voo satisfizer o predicado; false caso contrário.
     */
    virtual bool evaluate(const Flight &flight) {
        if (field == "org") {
            int cmp = strcmp(flight.origin, strValue.c_str());
            switch(op) {
                case EQ: return cmp == 0;
                case NE: return cmp != 0;
                case LT: return cmp < 0;
                case LE: return cmp <= 0;
                case GT: return cmp > 0;
                case GE: return cmp >= 0;
            }
        } else if (field == "dst") {
            int cmp = strcmp(flight.destination, strValue.c_str());
            switch(op) {
                case EQ: return cmp == 0;
                case NE: return cmp != 0;
                case LT: return cmp < 0;
                case LE: return cmp <= 0;
                case GT: return cmp > 0;
                case GE: return cmp >= 0;
            }
        } else if (field == "prc") {
            switch(op) {
                case EQ: return flight.price == numValue;
                case NE: return flight.price != numValue;
                case LT: return flight.price < numValue;
                case LE: return flight.price <= numValue;
                case GT: return flight.price > numValue;
                case GE: return flight.price >= numValue;
            }
        } else if (field == "dur") {
            int value = static_cast<int>(numValue);
            switch(op) {
                case EQ: return flight.duration == value;
                case NE: return flight.duration != value;
                case LT: return flight.duration < value;
                case LE: return flight.duration <= value;
                case GT: return flight.duration > value;
                case GE: return flight.duration >= value;
            }
        } else if (field == "sto") {
            int value = static_cast<int>(numValue);
            switch(op) {
                case EQ: return flight.stops == value;
                case NE: return flight.stops != value;
                case LT: return flight.stops < value;
                case LE: return flight.stops <= value;
                case GT: return flight.stops > value;
                case GE: return flight.stops >= value;
            }
        } else if (field == "sea") {  // Assentos disponíveis
            int value = static_cast<int>(numValue);
            switch(op) {
                case EQ: return flight.seats == value;
                case NE: return flight.seats != value;
                case LT: return flight.seats < value;
                case LE: return flight.seats <= value;
                case GT: return flight.seats > value;
                case GE: return flight.seats >= value;
            }
        } else if (field == "dep") {  // Data/hora de partida
            time_t value = static_cast<time_t>(numValue);
            switch(op) {
                case EQ: return flight.dep_time == value;
                case NE: return flight.dep_time != value;
                case LT: return flight.dep_time < value;
                case LE: return flight.dep_time <= value;
                case GT: return flight.dep_time > value;
                case GE: return flight.dep_time >= value;
            }
        } else if (field == "arr") {  // Data/hora de chegada
            time_t value = static_cast<time_t>(numValue);
            switch(op) {
                case EQ: return flight.arr_time == value;
                case NE: return flight.arr_time != value;
                case LT: return flight.arr_time < value;
                case LE: return flight.arr_time <= value;
                case GT: return flight.arr_time > value;
                case GE: return flight.arr_time >= value;
            }
        }
        return false;
    }

    virtual ~PredicateExpr() { }
};

#endif // EXPRESSION_HPP