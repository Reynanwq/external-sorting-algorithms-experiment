// funcoes_gerais.cpp
#include "funcoes_gerais.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;

void minhaFuncaoGeral() {
    cout << "Minha função Geral foi chamada!" << endl;
}

double calcularAlfa(int escritasArquivo, int nRegistros) {
    return round((escritasArquivo / nRegistros) * 100) / 100;
}

vector<int> gerarDadosAleatorios(int n) {
    vector<int> dados(n);
    srand(static_cast<unsigned>(time(0))); // Semente para números aleatórios
    for (int& valor : dados) {
        valor = rand() % 100; // Números aleatórios entre 0 e 99
    }
    return dados;
}
