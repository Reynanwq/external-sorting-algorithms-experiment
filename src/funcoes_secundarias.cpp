// funcoes_secundarias.cpp
#include "funcoes_secundarias.h"
#include "estruturas.h"
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void minhaFuncaoSecundaria() {
    cout << "Minha função secundaria foi chamada!" << endl;
}

// Funções Secundárias auxiliares
void exibirListaDeListasInt(vector<vector<int>>& listas) {
    int contador = 0;
    for (vector<int> lista : listas) {
        cout << contador << ": { ";
        for (int valor : lista) {
            cout << valor << " ";
        }
        cout << " }" << endl;
        contador++;
    }
    cout << "-----------------------------------------" << endl << endl << endl;
}

void exibirListaInt(vector<int>& lista, string nome) {
    cout <<  nome << ": ";
    for (int valor : lista ) {
        cout << valor << " ";
    }
    cout << endl;
}

void exibirHeap(priority_queue<Element, vector<Element>, Compare> heap) {
    cout << "Heap: ";
    while (!heap.empty())
    {
        if (!heap.top().marked)
            cout << heap.top().value << " ";
        else
            cout << heap.top().value << "* ";
        
        heap.pop();
    }
    cout << endl;
}

void exibirListaDeMatrizValorBinary(vector<vector<vector<ValorBinary>>>& listaDeMatriz) {
    for (unsigned long i = 0; i < listaDeMatriz.size(); i++) {
        // cout << "Matriz Exibir\n";
        for (unsigned long j = 0; j < listaDeMatriz[i].size(); j++) {
            if (listaDeMatriz[i][j].size() != 0){
                cout << "Página " << i << endl << "Sequencia: " << j << endl;
                for (unsigned long k = 0; k < listaDeMatriz[i][j].size(); k++) {
                    cout << listaDeMatriz[i][j][k].valor << " ";
                }
                cout << endl << endl;
            }
        }
    }
}

void exibirValorBinary(const ValorBinary& registro) {
    cout << "Ocupado: " << registro.ocupado << endl;
    cout << "Registro Falso: " << registro.registroFalso << endl;
    cout << "Valor: " << registro.valor << endl;
    cout << "Sequencia: " << registro.sequencia << endl;
    cout << "Tamaho Sequencia: " << registro.tamSequencia << endl;
    cout << "Posicao: " << registro.posicao << endl;
    cout << "Arquivo de Origem: " << registro.arquivoOrigem << endl << endl;
}
