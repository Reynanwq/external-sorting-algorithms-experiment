// funcoes_secundarias.h
#ifndef FUNCOES_SECUNDARIAS_H
#define FUNCOES_SECUNDARIAS_H

#include <vector>
#include "estruturas.h"

using namespace std;

// Teste
void minhaFuncaoSecundaria();

// Funções Secundárias auxiliares
void exibirListaDeListasInt(vector<vector<int>>& listas);
void exibirListaInt(vector<int>& lista, string nome);
void exibirHeap(priority_queue<Element, vector<Element>, Compare> heap);
void exibirListaDeMatrizValorBinary(vector<vector<vector<ValorBinary>>>& listaDeMatriz);
void exibirValorBinary(const ValorBinary& registro);

#endif // FUNCOES_SECUNDARIAS_H