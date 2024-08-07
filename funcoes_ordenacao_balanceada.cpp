// funcoes_ordenacao_balanceada.cpp
#include "funcoes_ordenacao_balanceada.h"
#include <iostream>

using namespace std;

void minhaFuncaoBalanceada() {
    cout << "Minha função balanceada foi chamada!" << endl;
}

// Imprimir saída ordenação balanceada
void imprimirSaida(int fase, float beta, const vector<vector<vector<int>>>& sequencias){
    cout << "fase " << fase << " " << beta << endl;
    for (int i = 0; i < sequencias.size(); i++) {
        if (sequencias[i].size()) {
            cout << i + 1 << ": ";
            for (int j = 0; j < sequencias[i].size(); j++){
                for (int k = 0; k < sequencias[i][j].size(); k++) {
                    if (k == 0){
                        cout << "{" << sequencias[i][j][k] << " ";
                    } else if(k < sequencias[i][j].size() - 1) {
                        cout << sequencias[i][j][k] << " ";
                    } else {
                        cout << sequencias[i][j][k] << "}";
                    }
                }
            }
            cout << endl;

        }
    }
}

void imprimirSaidaIncremento(int fase, float beta, const vector<vector<vector<int>>>& sequencias, int incrementoPaginas){
    cout << "fase " << fase << " " << beta << endl;
    for (int i = 0; i < sequencias.size(); i++) {
        if (sequencias[i].size()) {
            cout << i + 1 + incrementoPaginas << ": ";
            for (int j = 0; j < sequencias[i].size(); j++){
                for (int k = 0; k < sequencias[i][j].size(); k++) {
                    if (k == 0){
                        cout << "{" << sequencias[i][j][k] << " ";
                    } else if(k < sequencias[i][j].size() - 1) {
                        cout << sequencias[i][j][k] << " ";
                    } else {
                        cout << sequencias[i][j][k] << "}";
                    }
                }
            }
            cout << endl;
        }
    }
}