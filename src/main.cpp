#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdio>
#include "../include/DateTime.hpp"
#include "../include/Flight.hpp"
#include "../include/Parser.hpp"
#include "../include/Expression.hpp"
#include "../include/AVLTree.hpp"
#include "../include/Sort.hpp"

using namespace std;

/**
 * @brief Compara dois valores do tipo double.
 */
int compareDoubles(const double &a, const double &b) {
    if (a < b) return -1;
    else if (a > b) return 1;
    else return 0;
}

/**
 * @brief Compara dois inteiros.
 */
int compareInts(const int &a, const int &b) {
    if (a < b) return -1;
    else if (a > b) return 1;
    else return 0;
}

/**
 * @brief Compara duas strings.
 */
int compareStrings(const string &a, const string &b) {
    return a.compare(b);
}

/**
 * @brief Compara dois valores do tipo time_t.
 */
int compareTimes(const time_t &a, const time_t &b) {
    if(a < b) return -1;
    else if(a > b) return 1;
    return 0;
}

// Árvores AVL globais para indexar os voos por diferentes campos.
AVLTree<string>* indexOrigin = nullptr;
AVLTree<string>* indexDestination = nullptr;
AVLTree<double>* indexPrice = nullptr;
AVLTree<int>* indexDuration = nullptr;
AVLTree<int>* indexStops = nullptr;
AVLTree<int>* indexSeats = nullptr;
AVLTree<time_t>* indexDeparture = nullptr;
AVLTree<time_t>* indexArrival = nullptr;

/**
 * @brief Gerenciador de voos que constrói os índices.
 */
class FlightManager {
public:
    Flight* flights;    ///< Array de voos.
    int flightCount;    ///< Número de voos.

    /**
     * @brief Construtor.
     * @param flightArray Array de voos.
     * @param count Número de voos.
     */
    FlightManager(Flight* flightArray, int count) : flights(flightArray), flightCount(count) {}

    /**
     * @brief Constrói os índices (árvores AVL) para os campos.
     */
    void buildIndices() {
        indexOrigin = new AVLTree<string>(compareStrings);
        indexDestination = new AVLTree<string>(compareStrings);
        indexPrice = new AVLTree<double>(compareDoubles);
        indexDuration = new AVLTree<int>(compareInts);
        indexStops = new AVLTree<int>(compareInts);
        indexSeats = new AVLTree<int>(compareInts);
        indexDeparture = new AVLTree<time_t>(compareTimes);
        indexArrival = new AVLTree<time_t>(compareTimes);
        
        for (int i = 0; i < flightCount; i++) {
            indexOrigin->insert(string(flights[i].origin), &flights[i]);
            indexDestination->insert(string(flights[i].destination), &flights[i]);
            indexPrice->insert(flights[i].price, &flights[i]);
            indexDuration->insert(flights[i].duration, &flights[i]);
            indexStops->insert(flights[i].stops, &flights[i]);
            indexSeats->insert(flights[i].seats, &flights[i]);
            indexDeparture->insert(flights[i].dep_time, &flights[i]);
            indexArrival->insert(flights[i].arr_time, &flights[i]);
        }
    }
    
    /**
     * @brief Destrutor: libera os índices.
     */
    ~FlightManager() {
        delete indexOrigin;
        delete indexDestination;
        delete indexPrice;
        delete indexDuration;
        delete indexStops;
        delete indexSeats;
        delete indexDeparture;
        delete indexArrival;
    }
};

/**
 * @brief Procura recursivamente um predicado indexável na árvore de expressão.
 *
 * Apenas predicados com operador diferente de NE e em campos indexáveis são considerados.
 *
 * @param expr Ponteiro para a expressão.
 * @return Ponteiro para o PredicateExpr indexável, ou nullptr se não houver.
 */
PredicateExpr* findIndexablePredicate(Expr* expr) {
    if (!expr)
        return nullptr;
    PredicateExpr* predicate = dynamic_cast<PredicateExpr*>(expr);
    if (predicate) {
        if (predicate->op != PredicateExpr::NE &&
            (predicate->field == "org" || predicate->field == "dst" ||
             predicate->field == "prc" || predicate->field == "dur" || predicate->field == "sto" ||
             predicate->field == "sea" || predicate->field == "dep" || predicate->field == "arr"))
            return predicate;
        return nullptr;
    }
    BinaryExpr* binaryExpr = dynamic_cast<BinaryExpr*>(expr);
    if (binaryExpr) {
        if (binaryExpr->op == '&') {
            PredicateExpr* leftPredicate = findIndexablePredicate(binaryExpr->left);
            if (leftPredicate)
                return leftPredicate;
            return findIndexablePredicate(binaryExpr->right);
        } else {
            return nullptr;
        }
    }
    return nullptr;
}

/**
 * @brief Retorna candidatos usando o índice, conforme o predicado.
 *
 * @param predicate Predicado indexável.
 * @param candidateCount (Saída) Número de candidatos encontrados.
 * @return Array dinamicamente alocado de ponteiros para Flight (deve ser liberado pelo chamador).
 */
Flight** getCandidatesFromIndex(PredicateExpr* predicate, int &candidateCount) {
    if (predicate->field == "prc") {
        double value = predicate->numValue;
        if (predicate->op == PredicateExpr::EQ)
            return indexPrice->rangeQuery(&value, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::LT)
            return indexPrice->rangeQuery(nullptr, true, &value, false, candidateCount);
        else if (predicate->op == PredicateExpr::LE)
            return indexPrice->rangeQuery(nullptr, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::GT)
            return indexPrice->rangeQuery(&value, false, nullptr, true, candidateCount);
        else if (predicate->op == PredicateExpr::GE)
            return indexPrice->rangeQuery(&value, true, nullptr, true, candidateCount);
    }
    else if (predicate->field == "dur") {
        int value = static_cast<int>(predicate->numValue);
        if (predicate->op == PredicateExpr::EQ)
            return indexDuration->rangeQuery(&value, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::LT)
            return indexDuration->rangeQuery(nullptr, true, &value, false, candidateCount);
        else if (predicate->op == PredicateExpr::LE)
            return indexDuration->rangeQuery(nullptr, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::GT)
            return indexDuration->rangeQuery(&value, false, nullptr, true, candidateCount);
        else if (predicate->op == PredicateExpr::GE)
            return indexDuration->rangeQuery(&value, true, nullptr, true, candidateCount);
    }
    else if (predicate->field == "sto") {
        int value = static_cast<int>(predicate->numValue);
        if (predicate->op == PredicateExpr::EQ)
            return indexStops->rangeQuery(&value, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::LT)
            return indexStops->rangeQuery(nullptr, true, &value, false, candidateCount);
        else if (predicate->op == PredicateExpr::LE)
            return indexStops->rangeQuery(nullptr, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::GT)
            return indexStops->rangeQuery(&value, false, nullptr, true, candidateCount);
        else if (predicate->op == PredicateExpr::GE)
            return indexStops->rangeQuery(&value, true, nullptr, true, candidateCount);
    }
    else if (predicate->field == "sea") {
        int value = static_cast<int>(predicate->numValue);
        if (predicate->op == PredicateExpr::EQ)
            return indexSeats->rangeQuery(&value, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::LT)
            return indexSeats->rangeQuery(nullptr, true, &value, false, candidateCount);
        else if (predicate->op == PredicateExpr::LE)
            return indexSeats->rangeQuery(nullptr, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::GT)
            return indexSeats->rangeQuery(&value, false, nullptr, true, candidateCount);
        else if (predicate->op == PredicateExpr::GE)
            return indexSeats->rangeQuery(&value, true, nullptr, true, candidateCount);
    }
    else if (predicate->field == "dep") {
        time_t value = static_cast<time_t>(predicate->numValue);
        if (predicate->op == PredicateExpr::EQ)
            return indexDeparture->rangeQuery(&value, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::LT)
            return indexDeparture->rangeQuery(nullptr, true, &value, false, candidateCount);
        else if (predicate->op == PredicateExpr::LE)
            return indexDeparture->rangeQuery(nullptr, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::GT)
            return indexDeparture->rangeQuery(&value, false, nullptr, true, candidateCount);
        else if (predicate->op == PredicateExpr::GE)
            return indexDeparture->rangeQuery(&value, true, nullptr, true, candidateCount);
    }
    else if (predicate->field == "arr") {
        time_t value = static_cast<time_t>(predicate->numValue);
        if (predicate->op == PredicateExpr::EQ)
            return indexArrival->rangeQuery(&value, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::LT)
            return indexArrival->rangeQuery(nullptr, true, &value, false, candidateCount);
        else if (predicate->op == PredicateExpr::LE)
            return indexArrival->rangeQuery(nullptr, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::GT)
            return indexArrival->rangeQuery(&value, false, nullptr, true, candidateCount);
        else if (predicate->op == PredicateExpr::GE)
            return indexArrival->rangeQuery(&value, true, nullptr, true, candidateCount);
    }
    else if (predicate->field == "org") {
        string value = predicate->strValue;
        if (predicate->op == PredicateExpr::EQ)
            return indexOrigin->rangeQuery(&value, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::LT)
            return indexOrigin->rangeQuery(nullptr, true, &value, false, candidateCount);
        else if (predicate->op == PredicateExpr::LE)
            return indexOrigin->rangeQuery(nullptr, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::GT)
            return indexOrigin->rangeQuery(&value, false, nullptr, true, candidateCount);
        else if (predicate->op == PredicateExpr::GE)
            return indexOrigin->rangeQuery(&value, true, nullptr, true, candidateCount);
    }
    else if (predicate->field == "dst") {
        string value = predicate->strValue;
        if (predicate->op == PredicateExpr::EQ)
            return indexDestination->rangeQuery(&value, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::LT)
            return indexDestination->rangeQuery(nullptr, true, &value, false, candidateCount);
        else if (predicate->op == PredicateExpr::LE)
            return indexDestination->rangeQuery(nullptr, true, &value, true, candidateCount);
        else if (predicate->op == PredicateExpr::GT)
            return indexDestination->rangeQuery(&value, false, nullptr, true, candidateCount);
        else if (predicate->op == PredicateExpr::GE)
            return indexDestination->rangeQuery(&value, true, nullptr, true, candidateCount);
    }
    return nullptr;
}

/**
 * @brief Função principal.
 */
int main(int argc, char* argv[]) {
    if (argc > 1) {
        ifstream inputFile(argv[1]);
        if (!inputFile) {
            cerr << "Error opening file " << argv[1] << ".\n";
            return 1;
        }

        cin.rdbuf(inputFile.rdbuf());

        int flightCount;
        if (!(cin >> flightCount)) {
            cerr << "Error reading number of flights.\n";
            return 1;
        }

        Flight* flights = new Flight[flightCount];

        for (int i = 0; i < flightCount; i++) {
            if (!(cin >> flights[i].origin >> flights[i].destination >> flights[i].price 
                    >> flights[i].seats >> flights[i].departureStr >> flights[i].arrivalStr 
                    >> flights[i].stops)) {
                cerr << "Error reading flight " << i + 1 << ".\n";
                return 1;
            }

            flights[i].dep_time = parseDateTime(flights[i].departureStr);
            flights[i].arr_time = parseDateTime(flights[i].arrivalStr);
            
            if (flights[i].arr_time < flights[i].dep_time) {
                cerr << "Error: arrival time is before departure for flight " << i + 1 << ".\n";
                return 1;
            }
            
            flights[i].duration = static_cast<int>(flights[i].arr_time - flights[i].dep_time);
        }

        FlightManager flightManager(flights, flightCount);
        flightManager.buildIndices();

        int queryCount;
        if (!(cin >> queryCount)) {
            cerr << "Error reading number of queries.\n";
            return 1;
        }
        
        cin.ignore();  // Ignora '\n'

        for (int i = 0; i < queryCount; i++) {
            string queryLine;
            while (queryLine.empty()) {
                if (!getline(cin, queryLine)) {
                    cerr << "Error reading query " << i + 1 << ".\n";
                    return 1;
                }
            }

            istringstream queryStream(queryLine);
            int maxResults;
            string sortCriteria;

            if (!(queryStream >> maxResults >> sortCriteria)) {
                cerr << "Error parsing query " << i + 1 << ".\n";
                return 1;
            }

            string expressionStr;
            getline(queryStream, expressionStr);
            while (!expressionStr.empty() && isspace(expressionStr[0]))
                expressionStr.erase(expressionStr.begin());

            printf("%d %s %s\n", maxResults, sortCriteria.c_str(), expressionStr.c_str());

            Parser parser(expressionStr);
            Expr* expression = parser.parseExpression();

            PredicateExpr* candidatePredicate = findIndexablePredicate(expression);
            Flight** candidateFlights = nullptr;
            int candidateCount = 0;
            bool indexUsed = false;

            if (candidatePredicate) {
                candidateFlights = getCandidatesFromIndex(candidatePredicate, candidateCount);
                indexUsed = true;
            } else {
                candidateCount = flightCount;
                candidateFlights = new Flight*[flightCount];
                for (int j = 0; j < flightCount; j++)
                    candidateFlights[j] = &flights[j];
                indexUsed = false;
            }

            int resultCount = 0;
            int resultCapacity = (candidateCount > 10) ? candidateCount : 10;
            Flight** resultFlights = new Flight*[resultCapacity];

            for (int j = 0; j < candidateCount; j++) {
                if (expression->evaluate(*candidateFlights[j])) {
                    if (resultCount >= resultCapacity) {
                        int newCapacity = resultCapacity * 2;
                        Flight** newArray = new Flight*[newCapacity];
                        for (int k = 0; k < resultCount; k++)
                            newArray[k] = resultFlights[k];
                        delete[] resultFlights;
                        resultFlights = newArray;
                        resultCapacity = newCapacity;
                    }
                    resultFlights[resultCount++] = candidateFlights[j];
                }
            }

            if (resultCount > 0)
                quickSortFlights(resultFlights, 0, resultCount - 1, sortCriteria);

            for (int j = 0; j < resultCount && j < maxResults; j++) {
                Flight* f = resultFlights[j];
                printf("%s %s %g %d %s %s %d\n", f->origin, f->destination, f->price, f->seats, f->departureStr, f->arrivalStr, f->stops);
            }

            delete[] resultFlights;
            if (indexUsed)
                delete[] candidateFlights;
            delete expression;
        }

        delete[] flights;
        return 0;
    } else {
        cerr << "Usage: ./bin/tp3.out input.txt\n";
        return 1;
    }
}