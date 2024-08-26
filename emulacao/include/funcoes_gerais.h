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
vector<vector<int>> gerarSequenciasIniciaisSimulacao(int m, int r);
void desmarcarTodosElementos(priority_queue<Element, vector<Element>, Compare>& heap);

// Manipulando arquivos
void criarArquivos(int n);
void criarLimparArquivosRange(int inicio, int fim);
bool apenasUmArquivoPreenchido(int n);

vector<fstream> criarArquivosDB(const fs::path& folder, const int& arquivosAbertos);
vector<fstream> abrirArquivosDB(const fs::path& folder, const int& arquivosAbertos);
fstream abrirArquivoDB(const fs::path& folder, const int& indiceArquivo);

void fecharArquivosDB(vector<fstream>& arquivos);
fstream truncarArquivoDB(fstream& arquivoAberto, int& indiceArquivo);
bool arquivoVazio(fstream& arquivo);
int qualOPrimeiroArquivoVazio(vector<fstream>& arquivos);
vector<int> quaisArquivosEstaoVazios(vector<fstream>& arquivos);
int quantosArquivosCheios(vector<fstream>& arquivos);

vector<vector<ValorBinary>> salvarArquivoNaMemoria(fstream& arquivo);
vector<vector<vector<ValorBinary>>> salvarEstadoDosArquivosNaMemoria(vector<fstream>& arquivos);

ValorBinary lerRegistroDoArquivo(fstream& arquivo, ArquivoIO& arquivoIO);
bool arquivoEstaEstavel(const fstream& arquivo);

void retornarPonteiroDeLeituraUmRegistro(fstream& arquivo, const auto& registro);

bool escreverRegistroNoArquivo(fstream& arquivo, ValorBinary& registro, ArquivoIO& arquivoIO);
bool escreverRegistroNoArquivoCopy(fstream& arquivo, ValorBinary& registro, ArquivoIO& arquivoIO);
bool escreverRegistroDesocupadoNoArquivo(fstream& arquivo, const ValorBinary& registro, ArquivoIO& arquivoIO);
bool arquivoEstaAberto(const fstream& arquivo);
priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> lerRegistrosIniciais(vector<fstream>& arquivos, const int& indiceArquivoVazio, vector<ValorBinary>& ultimoRegistroLidoDeCadaArquivo, vector<ArquivoIO> arquivosIO);
int esvaziarFilaDePrioridade(priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary>& heap, fstream& arquivo, const ValorBinary& registro, vector<ArquivoIO>& arquivosIO);

int quantosArquivosChegaramEOF(const vector<ArquivoIO>& arquivoIO);
ArquivoIO qualArquivoChegouEmEOF(const vector<ArquivoIO>& arquivoIO);
int quantosArquivosChocaramSequencias(const vector<ArquivoIO>& arquivoIO);
int somarTamanhoSequencias(const vector<ValorBinary>& registros);
void limparBufferStreamVector(vector<fstream>& arquivos);
void limparBufferStream(fstream& arquivo);

bool copiarSequenciasTodosOsArquivos(vector<fstream>& arquivos, vector<ArquivoIO>& arquivosIO);
bool copiarSequenciasRestantesProInicio(fstream& arquivo, ArquivoIO& arquivoIO);
bool truncarArquivoCopy(ArquivoIO arquivoIO, int registrosCopiados);

// Manipulando pastas
fs::path criarPasta(string nomePasta);
fs::path pegarPastaPages();

// Imprimindo resultados
void imprimirResultadoValorBinary(const vector<vector<vector<ValorBinary>>>& listaDeMatriz, const int& fase, const double& beta);

// calculando para saída
int quantidadeDeRegistrosValorBinary(const vector<vector<vector<ValorBinary>>>& arquivosEmMemoria);
int quantidadeDeSequenciasValorBinary(const vector<vector<vector<ValorBinary>>>& arquivosEmMemoria);

// TESTE_FUNCTION
void teste_function(vector<fstream>& arquivos);

#endif // FUNCOES_GERAIS_H