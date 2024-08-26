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

void exibirPonteirosArquivos(vector<fstream>& arquivos);
void exibirPonteiroArquivo(fstream& arquivo);

void exibirResultadoValorBinary(const vector<vector<vector<ValorBinary>>>& listaDeMatriz);

void exibirArquivosIO(vector<ArquivoIO> arquivosIO);

void olharArquivos(vector<fstream>& arquivos);

void olharPonteirosMemoria(vector<fstream>& arquivos);

void olharPonteirosMemoriaIO(vector<fstream>& arquivos, vector<ArquivoIO>& arquivosIO);

bool arquivosEstaveis(vector<fstream>& arquivos);

void exibirPilhaValorBinary(priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> heapValorBinary);

#endif // FUNCOES_SECUNDARIAS_H