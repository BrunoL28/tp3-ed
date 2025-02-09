#ifndef PARSER_HPP
#define PARSER_HPP

#include "Expression.hpp"
#include "DateTime.hpp"
#include <string>
#include <cctype>
#include <cstdlib>
#include <iostream>

using std::string;

/**
 * @brief Parser para expressões.
 */
class Parser {
public:
    string input;   ///< String de entrada a ser analisada.
    int position;   ///< Posição atual na string.

    /**
     * @brief Construtor.
     * @param str String de entrada.
     */
    Parser(const string &str) : input(str), position(0) {}

    /**
     * @brief Retorna o caractere atual sem avançar.
     * @return Caractere atual ou '\0' se no fim da string.
     */
    char peek() {
        if (position < static_cast<int>(input.size()))
            return input[position];
        return '\0';
    }

    /**
     * @brief Retorna o caractere atual e avança a posição.
     * @return Caractere atual ou '\0' se no fim da string.
     */
    char get() {
        if (position < static_cast<int>(input.size()))
            return input[position++];
        return '\0';
    }

    /**
     * @brief Pula os caracteres de espaço.
     */
    void skipWhitespace() {
        while (position < static_cast<int>(input.size()) && isspace(input[position]))
            position++;
    }

    /**
     * @brief Tenta casar um token na posição atual.
     * @param token Token a ser casado.
     * @return true se casou; false caso contrário.
     */
    bool match(const string &token) {
        skipWhitespace();
        int len = token.size();
        if (input.substr(position, len) == token) {
            position += len;
            return true;
        }
        return false;
    }

    /**
     * @brief Lê um identificador.
     * @return Identificador lido.
     */
    string parseIdentifier() {
        skipWhitespace();
        string identifier;
        while (position < static_cast<int>(input.size()) && isalpha(input[position])) {
            identifier.push_back(input[position]);
            position++;
        }
        return identifier;
    }

    /**
     * @brief Lê uma string numérica.
     * @return String contendo o número.
     */
    string parseNumberString() {
        skipWhitespace();
        string numberStr;
        while (position < static_cast<int>(input.size()) && (isdigit(input[position]) || input[position]=='.')) {
            numberStr.push_back(input[position]);
            position++;
        }
        return numberStr;
    }

    /**
     * @brief Lê um token de data/hora.
     * @return Token de data/hora.
     */
    string parseTimeToken() {
        skipWhitespace();
        string token;
        while (position < static_cast<int>(input.size()) && !isspace(input[position]) && input[position] != ')') {
            token.push_back(input[position]);
            position++;
        }
        return token;
    }

    /**
     * @brief Inicia a análise da expressão.
     * @return Ponteiro para a árvore de expressão resultante.
     */
    Expr* parseExpression() { 
        return parseOr(); 
    }

private:
    /**
     * @brief Analisa uma expressão OR.
     * @return Ponteiro para a expressão analisada.
     */
    Expr* parseOr() {
        Expr* leftExpr = parseAnd();
        skipWhitespace();
        while (match("||")) {
            Expr* rightExpr = parseAnd();
            BinaryExpr* binaryExpr = new BinaryExpr();
            binaryExpr->op = '|';
            binaryExpr->left = leftExpr;
            binaryExpr->right = rightExpr;
            leftExpr = binaryExpr;
            skipWhitespace();
        }
        return leftExpr;
    }

    /**
     * @brief Analisa uma expressão AND.
     * @return Ponteiro para a expressão analisada.
     */
    Expr* parseAnd() {
        Expr* leftExpr = parseNot();
        skipWhitespace();
        while (match("&&")) {
            Expr* rightExpr = parseNot();
            BinaryExpr* binaryExpr = new BinaryExpr();
            binaryExpr->op = '&';
            binaryExpr->left = leftExpr;
            binaryExpr->right = rightExpr;
            leftExpr = binaryExpr;
            skipWhitespace();
        }
        return leftExpr;
    }

    /**
     * @brief Analisa uma expressão NOT.
     * @return Ponteiro para a expressão analisada.
     */
    Expr* parseNot() {
        skipWhitespace();
        if (match("!")) {
            NotExpr* notExpr = new NotExpr();
            notExpr->child = parseNot();
            return notExpr;
        } else {
            return parsePrimary();
        }
    }

    /**
     * @brief Analisa uma expressão primária.
     * @return Ponteiro para a expressão analisada.
     */
    Expr* parsePrimary() {
        skipWhitespace();
        if (match("(")) {
            Expr* expr = parseExpression();
            skipWhitespace();
            if (!match(")")) {
                std::cerr << "Error: expected ')' at position " << position << std::endl;
                exit(1);
            }
            return expr;
        } else {
            return parsePredicate();
        }
    }

    /**
     * @brief Analisa um predicado.
     * @return Ponteiro para o predicado analisado.
     */
    Expr* parsePredicate() {
        skipWhitespace();
        string fieldName = parseIdentifier();
        skipWhitespace();
        string opStr;
        if (match("==")) { opStr = "=="; }
        else if (match("!=")) { opStr = "!="; }
        else if (match("<=")) { opStr = "<="; }
        else if (match(">=")) { opStr = ">="; }
        else if (peek() == '<') { opStr = "<"; position++; }
        else if (peek() == '>') { opStr = ">"; position++; }
        else {
            std::cerr << "Error: expected comparison operator at position " << position << std::endl;
            exit(1);
        }
        skipWhitespace();
        bool numericField = !(fieldName == "org" || fieldName == "dst");
        PredicateExpr* predicate = new PredicateExpr();
        predicate->field = fieldName;
        if (opStr == "==") predicate->op = PredicateExpr::EQ;
        else if (opStr == "!=") predicate->op = PredicateExpr::NE;
        else if (opStr == "<") predicate->op = PredicateExpr::LT;
        else if (opStr == "<=") predicate->op = PredicateExpr::LE;
        else if (opStr == ">") predicate->op = PredicateExpr::GT;
        else if (opStr == ">=") predicate->op = PredicateExpr::GE;
        predicate->isNumeric = numericField;
        
        if (numericField) {
            if (fieldName == "dep" || fieldName == "arr") {
                string timeString = parseTimeToken();
                predicate->numValue = static_cast<double>(parseDateTime(timeString.c_str()));
            } else {
                string numberString = parseNumberString();
                predicate->numValue = atof(numberString.c_str());
            }
        } else {
            predicate->strValue = parseIdentifier();
        }
        return predicate;
    }
};

#endif // PARSER_HPP