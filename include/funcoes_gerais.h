// funcoes_gerais.h
#ifndef FUNCOES_GERAIS_H
#define FUNCOES_GERAIS_H

#include <vector>
#include <queue>
#include <fstream>
#include <filesystem>
#include "estruturas.h"

using namespace std;
namespace fs = std::filesystem;

// Teste
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
vector<fstream> criarArquivosDB(const fs::path& folder, const int& arquivosAbertos);
vector<fstream> abrirArquivosDB(const fs::path& folder, const int& arquivosAbertos);
void fecharArquivosDB(vector<fstream>& arquivos);
fstream truncarArquivoDB(fstream& arquivoAberto, int& indiceArquivo);
bool arquivoVazio(fstream& arquivo);
int qualArquivoEstaVazio(vector<fstream>& arquivos);

void criarLimparArquivosRange(int inicio, int fim);
bool apenasUmArquivoPreenchido(int n);

// Manipulando pastas
fs::path criarPasta(string nomePasta);
fs::path pegarPastaPages();

#endif // FUNCOES_GERAIS_H