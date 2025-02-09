#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include "Flight.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

/**
 * @brief Nó de uma lista encadeada para armazenar voos com chaves duplicadas.
 */
struct FlightListNode {
    Flight* flight;           ///< Ponteiro para um voo.
    FlightListNode* next;     ///< Ponteiro para o próximo nó.
    
    /**
     * @brief Construtor.
     * @param flightPtr Ponteiro para o voo.
     */
    FlightListNode(Flight* flightPtr) : flight(flightPtr), next(nullptr) {}
};

template<typename T>
/**
 * @brief Nó da árvore AVL.
 */
struct AVLTreeNode {
    T key;                        ///< Valor da chave.
    FlightListNode* flightList;   ///< Lista encadeada de voos com a mesma chave.
    AVLTreeNode* left;            ///< Ponteiro para o filho esquerdo.
    AVLTreeNode* right;           ///< Ponteiro para o filho direito.
    int height;                   ///< Altura do nó.
    
    /**
     * @brief Construtor.
     * @param keyValue Valor da chave.
     * @param flightPtr Ponteiro para o voo associado.
     */
    AVLTreeNode(const T& keyValue, Flight* flightPtr)
        : key(keyValue), left(nullptr), right(nullptr), height(1) {
        flightList = new FlightListNode(flightPtr);
    }
};

template<typename T>
/**
 * @brief Implementação da árvore AVL.
 */
class AVLTree {
public:
    AVLTreeNode<T>* root;                 ///< Nó raiz da árvore.
    int (*compare)(const T&, const T&);    ///< Ponteiro para função de comparação.

    /**
     * @brief Construtor.
     * @param cmpFunc Ponteiro para a função de comparação.
     */
    AVLTree(int (*cmpFunc)(const T&, const T&)) : root(nullptr), compare(cmpFunc) {}

    /**
     * @brief Insere um voo na árvore usando a chave fornecida.
     * @param key Valor da chave.
     * @param flightPtr Ponteiro para o voo.
     */
    void insert(const T& key, Flight* flightPtr) {
        root = insertRecursive(root, key, flightPtr);
    }

    /**
     * @brief Executa uma consulta por intervalo.
     *
     * Se os ponteiros para os limites forem nullptr, não há restrição inferior/superior.
     *
     * @param low Ponteiro para o limite inferior.
     * @param lowInclusive True se o limite inferior é inclusivo.
     * @param high Ponteiro para o limite superior.
     * @param highInclusive True se o limite superior é inclusivo.
     * @param count (Saída) Número de voos encontrados.
     * @return Array dinamicamente alocado de ponteiros para Flight (deve ser liberado pelo chamador).
     */
    Flight** rangeQuery(const T* low, bool lowInclusive,
                        const T* high, bool highInclusive, int &count) {
        int capacity = 10;
        Flight** resultArray = new Flight*[capacity];
        count = 0;
        rangeQueryRecursive(root, low, lowInclusive, high, highInclusive, resultArray, count, capacity);
        return resultArray;
    }

    /**
     * @brief Destrutor.
     */
    ~AVLTree() {
        destroyTree(root);
    }

private:
    /**
     * @brief Retorna a altura de um nó.
     * @param node Ponteiro para o nó.
     * @return Altura do nó ou 0 se for nullptr.
     */
    int getNodeHeight(AVLTreeNode<T>* node) {
        return node ? node->height : 0;
    }

    /**
     * @brief Calcula o fator de equilíbrio de um nó.
     * @param node Ponteiro para o nó.
     * @return Fator de equilíbrio.
     */
    int getBalanceFactor(AVLTreeNode<T>* node) {
        return node ? getNodeHeight(node->left) - getNodeHeight(node->right) : 0;
    }

    /**
     * @brief Atualiza a altura de um nó.
     * @param node Ponteiro para o nó.
     */
    void updateNodeHeight(AVLTreeNode<T>* node) {
        if (node) {
            int leftHeight = getNodeHeight(node->left);
            int rightHeight = getNodeHeight(node->right);
            node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
        }
    }

    /**
     * @brief Realiza uma rotação à direita.
     * @param y Nó a ser rotacionado.
     * @return Novo nó raiz após a rotação.
     */
    AVLTreeNode<T>* rotateRight(AVLTreeNode<T>* y) {
        AVLTreeNode<T>* x = y->left;
        AVLTreeNode<T>* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateNodeHeight(y);
        updateNodeHeight(x);
        return x;
    }

    /**
     * @brief Realiza uma rotação à esquerda.
     * @param x Nó a ser rotacionado.
     * @return Novo nó raiz após a rotação.
     */
    AVLTreeNode<T>* rotateLeft(AVLTreeNode<T>* x) {
        AVLTreeNode<T>* y = x->right;
        AVLTreeNode<T>* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateNodeHeight(x);
        updateNodeHeight(y);
        return y;
    }

    /**
     * @brief Função recursiva de inserção.
     * @param node Nó atual.
     * @param key Valor da chave a inserir.
     * @param flightPtr Ponteiro para o voo.
     * @return Nó atualizado após a inserção.
     */
    AVLTreeNode<T>* insertRecursive(AVLTreeNode<T>* node, const T& key, Flight* flightPtr) {
        if (!node)
            return new AVLTreeNode<T>(key, flightPtr);
        
        int cmpResult = compare(key, node->key);
        if (cmpResult == 0) {
            // Chave duplicada: adiciona o voo à lista.
            FlightListNode* newFlightNode = new FlightListNode(flightPtr);
            newFlightNode->next = node->flightList;
            node->flightList = newFlightNode;
            return node;
        } else if (cmpResult < 0) {
            node->left = insertRecursive(node->left, key, flightPtr);
        } else {
            node->right = insertRecursive(node->right, key, flightPtr);
        }
        
        updateNodeHeight(node);
        int balance = getBalanceFactor(node);

        // Caso Esquerda–Esquerda
        if (balance > 1 && compare(key, node->left->key) < 0)
            return rotateRight(node);
        // Caso Direita–Direita
        if (balance < -1 && compare(key, node->right->key) > 0)
            return rotateLeft(node);
        // Caso Esquerda–Direita
        if (balance > 1 && compare(key, node->left->key) > 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // Caso Direita–Esquerda
        if (balance < -1 && compare(key, node->right->key) < 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    /**
     * @brief Função recursiva para consulta por intervalo.
     *
     * Adiciona os voos que satisfazem o intervalo em resultArray.
     *
     * @param node Nó atual.
     * @param low Ponteiro para o limite inferior.
     * @param lowInclusive Se true, o limite inferior é inclusivo.
     * @param high Ponteiro para o limite superior.
     * @param highInclusive Se true, o limite superior é inclusivo.
     * @param resultArray Array para armazenar os resultados.
     * @param count (Saída) Número de voos armazenados até o momento.
     * @param capacity Capacidade atual do array.
     */
    void rangeQueryRecursive(AVLTreeNode<T>* node, const T* low, bool lowInclusive,
                             const T* high, bool highInclusive,
                             Flight** &resultArray, int &count, int &capacity) {
        if (!node)
            return;
        if (low) {
            int cmpLow = compare(node->key, *low);
            if (cmpLow < 0 || (!lowInclusive && cmpLow == 0)) {
                rangeQueryRecursive(node->right, low, lowInclusive, high, highInclusive, resultArray, count, capacity);
                return;
            }
        }
        if (high) {
            int cmpHigh = compare(node->key, *high);
            if (cmpHigh > 0 || (!highInclusive && cmpHigh == 0)) {
                rangeQueryRecursive(node->left, low, lowInclusive, high, highInclusive, resultArray, count, capacity);
                return;
            }
        }
        rangeQueryRecursive(node->left, low, lowInclusive, high, highInclusive, resultArray, count, capacity);
        FlightListNode* flightNode = node->flightList;
        while (flightNode) {
            if (count >= capacity) {
                int newCapacity = capacity * 2;
                Flight** newArray = new Flight*[newCapacity];
                for (int i = 0; i < count; i++)
                    newArray[i] = resultArray[i];
                delete[] resultArray;
                resultArray = newArray;
                capacity = newCapacity;
            }
            resultArray[count++] = flightNode->flight;
            flightNode = flightNode->next;
        }
        rangeQueryRecursive(node->right, low, lowInclusive, high, highInclusive, resultArray, count, capacity);
    }

    /**
     * @brief Libera recursivamente a memória da árvore.
     * @param node Nó atual.
     */
    void destroyTree(AVLTreeNode<T>* node) {
        if (!node)
            return;
        destroyTree(node->left);
        destroyTree(node->right);
        FlightListNode* current = node->flightList;
        while (current) {
            FlightListNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        delete node;
    }
};

#endif // AVLTREE_HPP