// funcoes_ordenacao_balanceada.h
#ifndef FUNCOES_ORDENACAO_BALANCEADA_H
#define FUNCOES_ORDENACAO_BALANCEADA_H

#include <vector>

using namespace std;

void minhaFuncaoBalanceada();

// Imprimir saída ordenação balanceada
void imprimirSaida(int fase, float beta, const vector<vector<vector<int>>>& sequencias);
void imprimirSaidaIncremento(int fase, float beta, const vector<vector<vector<int>>>& sequencias, int incrementoPaginas);

#endif // FUNCOES_ORDENACAO_BALANCEADA_H