// estruturas.h
#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <iostream>
#include <queue>
#include <vector>

// Struct Element para heap mínima
struct Element {
    int value;
    bool marked;

    Element(int v, bool m = false) : value(v), marked(m) {}
};

// Função de comparação personalizada para Element
struct Compare {
    bool operator()(const Element& a, const Element& b) const {
        if (a.marked != b.marked) {
            return a.marked > b.marked; // elementos marcados são maiores
        }
        return a.value > b.value; // se ambos são marcados ou não marcados, compare os valores
    }
};

// Estrutura para armazenar o valor e sua origem - intercalação
struct Valor {
    int valor;
    int origem;
    int linha;
};

// Comparador para a fila de prioridade da intercalação
struct Comparador {
    bool operator()(const Valor& a, const Valor& b) {
        return a.valor > b.valor; // Para min-heap
    }
};

#endif // ESTRUTURAS_H