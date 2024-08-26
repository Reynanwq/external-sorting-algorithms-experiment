// funcoes_ordenacao_polifasica.h
#ifndef FUNCOES_ORDENACAO_POLIFASICA_H
#define FUNCOES_ORDENACAO_POLIFASICA_H
#include "estruturas.h"
#include <vector>
#include <queue>

using namespace std;

void minhaFuncaoPolifasica();

vector<int> calcularDivisaoSequenciasIniciais(const int& qtdSequenciasIniciais, const int& arqAbertos);
int somaValoresLista(vector<int>& lista);
int salvarListasEmArquivosBinary(const vector<vector<int>>& listas, const int& arquivosAbertos, const int& maxSequenciasIniciais);
pair<int, double> mergePolifasico(vector<fstream>& arquivos, const int& memoria);
int mergePolifasicoRodada(vector<fstream>& arquivos, const int& memoria);

int mergeRodadaPolifasica(vector<fstream>& arquivos, vector<vector<vector<ValorBinary>>>& arquivosEmMemoria);
int qualSequenciaVazia(vector<vector<vector<ValorBinary>>>& arquivosEmMemoria);

vector<vector<vector<ValorBinary>>> inserirValoresNaHeap(
    vector<vector<vector<ValorBinary>>>& arquivosEmMemoria,
    priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary>& heap,
    const int& indiceVazio
);

int qualAMenorQUantidadeDeSequencias(vector<vector<vector<ValorBinary>>>& arquivosNaMemoria);

#endif // FUNCOES_ORDENACAO_POLIFASICA_H