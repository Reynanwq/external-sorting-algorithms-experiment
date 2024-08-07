// funcoes_gerais.h
#ifndef FUNCOES_GERAIS_H
#define FUNCOES_GERAIS_H

#include <vector>
#include <queue>
#include "estruturas.h"

using namespace std;

// Testes
void minhaFuncaoGeral();

// Reynan - Funções em uso
vector<int> gerarDadosAleatorios(int n);

// Reynan - Funções não utilizadas ainda
void intercalaSequencias(vector<int>& dados, int m, int k);
void calcularMetricas(const vector<int>& dados, int r, int k);
void exibirResultados(const vector<int>& dados, int r, int k);

// Calculo desempenho
double calcularAlfa(int escritasArquivo, int nRegistros);
double calcularBeta(int memoria, int nSequencias, int nRegistros);
int calcularQtdRegistros(const vector<vector<int>>& sequencias);
int calcularNumeroSequencias(const vector<vector<vector<int>>>& sequencias);

// Utilizando heap mínima
vector<vector<int>> gerarSequenciasIniciais(vector<int> dados, int m, int r);
void desmarcarTodosElementos(priority_queue<Element, vector<Element>, Compare>& heap);

// Manipulando arquivos
void criarArquivos(int n);
void criarLimparArquivosRange(int inicio, int fim);
bool apenasUmArquivoPreenchido(int n);

#endif // FUNCOES_GERAIS_H