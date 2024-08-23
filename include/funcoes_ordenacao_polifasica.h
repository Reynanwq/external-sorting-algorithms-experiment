// funcoes_ordenacao_polifasica.h
#ifndef FUNCOES_ORDENACAO_POLIFASICA_H
#define FUNCOES_ORDENACAO_POLIFASICA_H
#include <vector>

using namespace std;

void minhaFuncaoPolifasica();

vector<int> calcularDivisaoSequenciasIniciais(const int& qtdSequenciasIniciais, const int& arqAbertos);
int somaValoresLista(vector<int>& lista);
int salvarListasEmArquivosBinary(const vector<vector<int>>& listas, const int& arquivosAbertos, const int& maxSequenciasIniciais);
pair<int, double> mergePolifasico(vector<fstream>& arquivos, const int& memoria);
int mergePolifasicoRodada(vector<fstream>& arquivos, const int& memoria);

#endif // FUNCOES_ORDENACAO_POLIFASICA_H