// funcoes_ordenacao_balanceada.h
#ifndef FUNCOES_ORDENACAO_BALANCEADA_H
#define FUNCOES_ORDENACAO_BALANCEADA_H

#include <vector>

using namespace std;

void minhaFuncaoBalanceada();

// Imprimir saída ordenação balanceada
void imprimirSaida(int fase, float beta, const vector<vector<vector<int>>>& sequencias);
void imprimirSaidaIncremento(int fase, float beta, const vector<vector<vector<int>>>& sequencias, int incrementoPaginas);

// Salvar dados
void salvarListasEmArquivos(const vector<vector<int>>& listas, int arquivosIntercalados);
void salvarEstadoDasPaginas(int fase, int arquivosIntercalados, vector<vector<vector<int>>>& dadosEstado);

// Merge arquivos fases
void mergeArquivosFaseUm(int& paginasMerge, int& escritasArquivo);
void mergeArquivosFaseDois(int paginasMerge, int& escritasArquivo);

// Intercalar arquivos
float intercalaSalvarCalcularBalanceada(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas );
void intercalaSequenciasFaseUmBalanceada(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo);
void intercalaSequenciasFaseDoisBalanceada(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo);

#endif // FUNCOES_ORDENACAO_BALANCEADA_H