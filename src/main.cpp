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
pair<float, float> ordenacaoBalanceada(vector<vector<int>>& sequencias, int memoria, int arquivosTotais, int r, int nRegistros, vector<pair<int, float>> listBetas, int& escritasArquivo);
pair<float, float> ordenacaoPolifasica(vector<vector<int>>& sequencias, const int& memoria, const int& arquivosAbertos, const int& sequenciasIniciais, const int& nRegistros);
pair<float, float> ordenacaoCascata(vector<int>& dados, int memoria, int arquivosAbertos, int sequenciasIniciais, int nRegistros);
// 
int main() {
    // Funções para teste de integração
    // minhaFuncaoGeral();
    // minhaFuncaoSecundaria();
    // minhaFuncaoBalanceada();
    // minhaFuncaoPolifasica();
    // minhaFuncaoCascata();

    // Exemplo de uso
    int m = 2;  // Capacidade da memória interna
    int k = 3;  // Número de arquivos abertos
    int r = 3;  // Número de sequências iniciais
    int n = 6; // Número de registros
    if(n ==17){}; // Evitar erro unusable variable replit


    // vector<int> dados = gerarDadosAleatorios(n);
    // vector<int> dados = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    // vector<int> dados; // = {4, 3, 1, 6, 5, 3, 3};
    // vector<int> dados1 = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    // dados.insert(dados.end(), dados1.begin(), dados1.end());

    // r = 6;

    char metodo;
    // int m, k, r, n;

    // Leitura da primeira linha
    cin >> metodo >> m >> k >> r >> n;

    // Vetor para armazenar os inteiros da segunda linha
    // vector<int> numeros(n);
    vector<int> dados(n);

    // Leitura da segunda linha de acordo com o valor de "quantidade"
    for (int i = 0; i < n; ++i) {
        cin >> dados[i];
    }


    vector<vector<int>> sequencias = gerarSequenciasIniciais(dados, m, r);
    vector<pair<int, float>> listBetas; // Recuperar valores de Beta para cada fase
    int nRegistros = calcularQtdRegistros(sequencias);
    int escritasArquivo = 0; // Para calcular alfa
    pair<float, float> alfaBeta;

    cout << fixed << setprecision(2);

    criarArquivos(k);

    // Escolher o método de ordenação
    // k = 4; // temp
    // char metodo = 'B';
    // char metodo = 'P'; // Pode ser 'B', 'P' ou 'C'

    switch (metodo) {
        case 'B':
            alfaBeta = ordenacaoBalanceada(sequencias, m, k, r, nRegistros, listBetas, escritasArquivo);
            break;
        case 'P':
            alfaBeta = ordenacaoPolifasica(sequencias, m, k, r, nRegistros);
            break;
        case 'C':
            alfaBeta = ordenacaoCascata(dados, m, k, r, nRegistros);
            break;
        default:
            cerr << "Método de ordenação inválido!" << endl;
            return 1;
    }

    // double alfa = calcularAlfa(escritasArquivo, nRegistros);
    cout << "final " << alfaBeta.first << endl;
    
    return 0;
}

// Função para ordenação balanceada multi-caminhos
pair<float, float> ordenacaoBalanceada(vector<vector<int>>& sequencias, int memoria, int arquivosTotais, int r, int nRegistros, vector<pair<int, float>> listBetas, int& escritasArquivo) {
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
    float alfa = calcularAlfa(escritasArquivo, nRegistros);
    return make_pair(alfa, listBetas[0].second);
}

// Função para ordenação polifásica
pair<float, float> ordenacaoPolifasica(vector<vector<int>>& sequencias, const int& memoria, const int& arquivosAbertos, const int& sequenciasIniciais, const int& nRegistros) {
    int escritasEmArquivo = 0;
    float alfa = 0, beta = 0;
    pair<int, double> retornoEscritasEBeta;
    
    escritasEmArquivo = salvarListasEmArquivosBinary(sequencias, arquivosAbertos, sequenciasIniciais);
    

    fs::path folder = criarPasta("pages");
    vector<fstream> arquivos = abrirArquivosDB(folder, arquivosAbertos);

    limparBufferStreamVector(arquivos);

    // teste(arquivos); // TESTE_FUNCTION

    retornoEscritasEBeta = mergePolifasico(arquivos, memoria);
    escritasEmArquivo += retornoEscritasEBeta.first;
    beta = retornoEscritasEBeta.second;

    cout << "Escritas em arquivo: " << escritasEmArquivo << endl;

    // RECALCULAR A QUANTIDADE DE REGISTROS TOTAIS UTILIZADA -> FAZER UM PAIR<INT, PAIR<DOUBLE, INT>> ESCRITA, BETA, REGISTROS

    alfa = calcularAlfa(escritasEmArquivo, nRegistros); // MUDAR REGISTROS
    return make_pair(alfa, beta);
}

// Função para ordenação em cascata
pair<float, float> ordenacaoCascata(vector<int>& dados, int memoria, int arquivosAbertos, int sequenciasIniciais, int nRegistros) {
    if (arquivosAbertos == 0 || nRegistros == 0){}; // temp - avoid flags
    int escritasArquivo = 0;
    float alfa = 0, beta = 0;
    
    gerarSequenciasIniciais(dados, memoria, sequenciasIniciais);
    // Implementar a ordenação em cascata
    cout << "Ordenação em Cascata ainda não implementada." << endl;

    alfa = calcularAlfa(escritasArquivo, nRegistros);
    return make_pair(alfa, beta);
}
