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

// Comparador para a fila de prioridade da intercalação balanceada
struct Comparador {
    bool operator()(const Valor& a, const Valor& b) {
        return a.valor > b.valor; // Para min-heap
    }
};

// Structs ordenação polifásica
struct ValorBinary {
    bool ocupado;
    bool registroFalso;
    int valor;
    int sequencia;
    int tamSequencia;
    int posicao;
    int arquivoOrigem;

    // Operador de igualdade
    bool operator==(const ValorBinary& other) const {
        return ocupado == other.ocupado &&
               registroFalso == other.registroFalso &&
               valor == other.valor &&
               sequencia == other.sequencia &&
               tamSequencia == other.tamSequencia &&
               posicao == other.posicao &&
               arquivoOrigem == other.arquivoOrigem;
    }
};

// Função de comparação
struct ComparadorValorBinary {
    bool operator()(const ValorBinary& a, const ValorBinary& b) {
        if (a.ocupado != b.ocupado) {
            // Se um dos dois estiver ocupado e o outro não, o que estiver ocupado é considerado menor
            return a.ocupado > b.ocupado; // True é considerado maior (menor na ordenação)
        }
        // Se ambos têm o mesmo valor para 'ocupado', compara os inteiros
        if (a.registroFalso != b.registroFalso) return a.registroFalso < b.registroFalso;
        if (a.valor != b.valor) return a.valor < b.valor;
        if (a.sequencia != b.sequencia) return a.sequencia < b.sequencia;
        if (a.tamSequencia != b.tamSequencia) return a.tamSequencia < b.tamSequencia;
        if (a.posicao != b.posicao) return a.posicao < b.posicao;
        return a.arquivoOrigem < b.arquivoOrigem;
    }
};

#endif // ESTRUTURAS_H