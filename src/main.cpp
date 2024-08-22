#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <string>
#include <cmath>

#include "funcoes_gerais.h"
#include "funcoes_secundarias.h"
#include "funcoes_ordenacao_balanceada.h"
#include "funcoes_ordenacao_polifasica.h"
#include "funcoes_ordenacao_cascata.h"
#include "estruturas.h"

using namespace std;
namespace fs = std::filesystem; // Alias para evitar digitar std::filesystem repetidamente

// Funções para os métodos de ordenação externa
void ordenacaoBalanceada(vector<vector<int>>& sequencias, int memoria, int arquivosTotais, int r, int nRegistros, vector<pair<int, float>> listBetas, int& escritasArquivo);
void ordenacaoPolifasica(vector<vector<int>>& sequencias, int memoria, int arquivosAbertos, int maxSequenciasIniciais);
void ordenacaoCascata(vector<int>& dados, int memoria, int arquivosAbertos, int maxSequenciasIniciais);
// 
int main() {
    // Funções para teste de integração
    // minhaFuncaoGeral();
    // minhaFuncaoSecundaria();
    // minhaFuncaoBalanceada();
    // minhaFuncaoPolifasica();
    // minhaFuncaoCascata();

    // Exemplo de uso
    int m = 3;  // Capacidade da memória interna
    int k = 4;  // Número de arquivos abertos
    int r = 3;  // Número de sequências iniciais
    int n = 17; // Número de registros
    if(n ==17){}; // Evitar erro unusable variable replit


    // vector<int> dados = gerarDadosAleatorios(n);
    vector<int> dados = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    vector<vector<int>> sequencias = gerarSequenciasIniciais(dados, m, r);
    vector<pair<int, float>> listBetas; // Recuperar valores de Beta para cada fase
    int nRegistros = calcularQtdRegistros(sequencias);
    int escritasArquivo = 0; // Para calcular alfa

    cout << fixed << setprecision(2);

    criarArquivos(k);

    // Escolher o método de ordenação
    // k = 3, r = 49; // temp
    char metodo = 'B';
    // char metodo = 'P'; // Pode ser 'B', 'P' ou 'C'

    switch (metodo) {
        case 'B':
            ordenacaoBalanceada(sequencias, m, k, r, nRegistros, listBetas, escritasArquivo);
            break;
        case 'P':
            ordenacaoPolifasica(sequencias, m, k, r);
            break;
        case 'C':
            ordenacaoCascata(dados, m, k, r);
            break;
        default:
            cerr << "Método de ordenação inválido!" << endl;
            return 1;
    }

    float alfa = calcularAlfa(escritasArquivo, nRegistros);
    cout << "final " << alfa << endl;
    
    return 0;
}

// Função para ordenação balanceada multi-caminhos
void ordenacaoBalanceada(vector<vector<int>>& sequencias, int memoria, int arquivosTotais, int r, int nRegistros, vector<pair<int, float>> listBetas, int& escritasArquivo) {
    if (r == 0){}; // temp - avoid flags
    
    int fase = 0;
    vector<vector<vector<int>>> estadoInicioFase;
    int arquivosIntercalados =  floor(arquivosTotais/2);
    salvarListasEmArquivos(sequencias, arquivosIntercalados);

    while (!apenasUmArquivoPreenchido(arquivosIntercalados)) {
        if (fase % arquivosIntercalados == 0) {
            intercalaSequenciasFaseUmBalanceada(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas, escritasArquivo);
        } else {
            intercalaSequenciasFaseDoisBalanceada(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas, escritasArquivo);
        }
        fase++;
    }

    float beta = intercalaSalvarCalcularBalanceada(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas);
    if (fase % arquivosIntercalados == 0) {
        imprimirSaida(fase, beta, estadoInicioFase);
    } else {
        imprimirSaidaIncremento(fase, beta, estadoInicioFase, arquivosIntercalados);
    }
}

// Função para ordenação polifásica
void ordenacaoPolifasica(vector<vector<int>>& sequencias, int memoria, int arquivosAbertos, int maxSequenciasIniciais) {
    if (memoria){} // temp - avoid flags

    // gerarSequenciasIniciais(dados, memoria, maxSequenciasIniciais);
    // Implementar a ordenação polifásica
    // cout << "Ordenação Polifásica ainda não implementada." << endl;

    
    vector<int> divisaoSequencias = calcularDivisaoSequenciasIniciais(maxSequenciasIniciais, arquivosAbertos);


}

// Função para ordenação em cascata
void ordenacaoCascata(vector<int>& dados, int memoria, int arquivosAbertos, int maxSequenciasIniciais) {
    if (arquivosAbertos == 0){}; // temp - avoid flags

    gerarSequenciasIniciais(dados, memoria, maxSequenciasIniciais);
    // Implementar a ordenação em cascata
    cout << "Ordenação em Cascata ainda não implementada." << endl;
}
