// estruturas.h
#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <iostream>
#include <queue>
#include <vector>
#include <limits>

using namespace std;

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
        return ocupado          == other.ocupado &&
               registroFalso    == other.registroFalso &&
               valor            == other.valor &&
               sequencia        == other.sequencia &&
               tamSequencia     == other.tamSequencia &&
               posicao          == other.posicao &&
               arquivoOrigem    == other.arquivoOrigem;
    }

    bool operator<(const ValorBinary& other) const {
        if (ocupado != other.ocupado) {
            // Se um dos dois estiver ocupado e o outro não, o que estiver ocupado é considerado menor
            return ocupado > other.ocupado; // True é considerado maior (menor na ordenação)
        }
        if (registroFalso != other.registroFalso) return registroFalso < other.registroFalso;
        // Se ambos têm o mesmo valor para 'ocupado' e 'registroFalso' compara os inteiros
        if (valor != other.valor) return valor < other.valor;
        return false;
        // if (sequencia != other.sequencia) return sequencia < other.sequencia;
        // if (tamSequencia != other.tamSequencia) return tamSequencia < other.tamSequencia;
        // if (posicao != other.posicao) return posicao < other.posicao;
        // return arquivoOrigem < other.arquivoOrigem;
    } // TESTE_MODIFICAR_AGORA

    struct Comparador {
        bool operator()(const ValorBinary& a, const ValorBinary& b) {
            return a < b;
        }
    };
    
    static ValorBinary criarInvalido() {
        ValorBinary valorBinary;
        valorBinary.ocupado = false;
        valorBinary.registroFalso = false;
        valorBinary.valor = numeric_limits<int>::max();
        valorBinary.sequencia = numeric_limits<int>::max();
        valorBinary.tamSequencia = numeric_limits<int>::max();
        valorBinary.posicao = numeric_limits<int>::max();
        valorBinary.arquivoOrigem = numeric_limits<int>::max();
        return valorBinary;
    }

    bool is_valido() const {
        ValorBinary valorBinary = this->criarInvalido();
        // cout << "this: " << this->posicao << endl; // TESTE_APAGAR
        // cout << "other: " << valorBinary.posicao << endl;
        // cout << ".is_valido()\n";
        // valorBinary.exibir();
        return !this->operator==(valorBinary);
    }

    void exibir() {
        cout << "Ocupado (bool): " << ocupado << endl;
        cout << "Registro falso (bool): " << registroFalso << endl;
        cout << "Valor (int): " << valor << endl;
        cout << "Sequencia (int): " << sequencia << endl;
        cout << "Tamanho sequencia (int): " << tamSequencia << endl;
        cout << "Posicao (int): " << posicao << endl;
        cout << "Arquivo origem (int): " << arquivoOrigem << endl << endl;
    }
};

// TESTE_MODIFICAR_AGORA
// // Função de comparação
// struct ComparadorValorBinary {
//     bool operator()(const ValorBinary& a, const ValorBinary& b) {
//         if (a.ocupado != b.ocupado) {
//             // Se um dos dois estiver ocupado e o outro não, o que estiver ocupado é considerado menor
//             return a.ocupado > b.ocupado; // True é considerado maior (menor na ordenação)
//         }
//         // Se ambos têm o mesmo valor para 'ocupado', compara os inteiros
//         if (a.registroFalso != b.registroFalso) return a.registroFalso < b.registroFalso;
//         if (a.valor != b.valor) return a.valor < b.valor;
//         if (a.sequencia != b.sequencia) return a.sequencia < b.sequencia;
//         if (a.tamSequencia != b.tamSequencia) return a.tamSequencia < b.tamSequencia;
//         if (a.posicao != b.posicao) return a.posicao < b.posicao;
//         return a.arquivoOrigem < b.arquivoOrigem;
//     }
// };

// Função de comparação
struct ComparadorValorBinary {
    bool operator()(const ValorBinary& a, const ValorBinary& b) {
        if (a.ocupado != b.ocupado) {
            // Se um dos dois estiver ocupado e o outro não, o que estiver ocupado é considerado menor
            return a.ocupado < b.ocupado; // True é considerado maior (menor na ordenação)
        }
        // Se ambos têm o mesmo valor para 'ocupado', compara os inteiros
        if (a.registroFalso != b.registroFalso) return a.registroFalso > b.registroFalso;
        if (a.valor != b.valor) return a.valor > b.valor;
        return false;
        // if (a.sequencia != b.sequencia) return a.sequencia > b.sequencia;
        // if (a.tamSequencia != b.tamSequencia) return a.tamSequencia > b.tamSequencia;
        // if (a.posicao != b.posicao) return a.posicao > b.posicao;
        // return a.arquivoOrigem > b.arquivoOrigem;
    }
};

struct ArquivoIO {
    int read = 0;
    int write = 0;
    int copy = 0;
    int sequencia = 0;
    int origem = 0;
    bool choqueSequencias = false;
    bool eof = false;

    void exibir(){
        cout << "Read (int): " << this->read << endl;
        cout << "Write (int): " << this->write << endl;
        cout << "Copy (int): " << this->copy << endl;
        cout << "Sequencia (int): " << this->sequencia << endl;
        cout << "Origem (int): " << this->origem << endl;
        cout << "Choque sequencias (bool): " << this->choqueSequencias << endl;
        cout << "EOF (bool): " << this->eof << endl << endl;
    }

    bool operator==(const ArquivoIO& other) const {
        return  read                == other.read &&
                write               == other.write &&
                copy                == other.copy &&
                sequencia           == other.sequencia &&
                origem              == other.origem &&
                choqueSequencias    == other.choqueSequencias &&
                eof                 == other.eof;
    }

    bool is_valido() const {
        ArquivoIO arquivoIO = this->criarInvalido();
        return !this->operator==(arquivoIO);
    }

    static vector<ArquivoIO> criarSetOrigem(int n) {
        vector<ArquivoIO> arquivosIO(n, ArquivoIO());

        for (unsigned long i = 0; i < arquivosIO.size(); i++) {
            arquivosIO[i].origem = i;
            cout << i << " ";
        }
        cout << endl;

        return arquivosIO;
    }

    static ArquivoIO criarInvalido(){
        ArquivoIO arquivoIO;
        arquivoIO.read = numeric_limits<int>::max();
        arquivoIO.write = numeric_limits<int>::max();
        arquivoIO.copy = numeric_limits<int>::max();
        arquivoIO.sequencia = numeric_limits<int>::max();
        arquivoIO.origem = numeric_limits<int>::max();
        arquivoIO.choqueSequencias = false;
        arquivoIO.eof = false;
        return arquivoIO;
    }

};

#endif // ESTRUTURAS_H