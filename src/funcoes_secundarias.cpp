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