// funcoes_ordenacao_polifasica.h
#ifndef FUNCOES_ORDENACAO_POLIFASICA_H
#define FUNCOES_ORDENACAO_POLIFASICA_H
#include <vector>

using namespace std;

void minhaFuncaoPolifasica();

vector<int> calcularDivisaoSequenciasIniciais(const int& qtdSequenciasIniciais, const int& arqAbertos);
int somaValoresLista(vector<int>& lista);
void salvarListasEmArquivos(const vector<vector<int>>& listas, const int& arquivosAbertos, const int& maxSequenciasIniciais);

#endif // FUNCOES_ORDENACAO_POLIFASICA_H