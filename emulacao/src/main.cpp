#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <string>
#include <cmath>

#include "matplotlibcpp.h"
#include "funcoes_gerais.h"
#include "funcoes_secundarias.h"
#include "funcoes_ordenacao_balanceada.h"
#include "funcoes_ordenacao_polifasica.h"
#include "funcoes_ordenacao_cascata.h"
#include "estruturas.h"

using namespace std;
namespace fs = std::filesystem; // Alias para evitar digitar std::filesystem repetidamente
namespace plt = matplotlibcpp;

// Funções para os métodos de ordenação externa
pair<float, float> ordenacaoBalanceada(vector<vector<int>>& sequencias, int memoria, int arquivosTotais, int r, int nRegistros, vector<pair<int, float>> listBetas, int& escritasArquivo);
pair<float, float> ordenacaoPolifasica(vector<vector<int>>& sequencias, const int& memoria, const int& arquivosAbertos, const int& sequenciasIniciais, const int& nRegistros);
pair<float, float> ordenacaoCascata(vector<int>& dados, int memoria, int arquivosAbertos, int sequenciasIniciais, int nRegistros);
// 
int main() {
    // plt::plot({1,2,3,4},"*");
    // plt::show();
    // Funções para teste de integração
    // minhaFuncaoGeral();
    // minhaFuncaoSecundaria();
    // minhaFuncaoBalanceada();
    // minhaFuncaoPolifasica();
    // minhaFuncaoCascata();

    vector<float> X;
    vector<float> Y;

    // Exemplo de uso
    // int m = 2;  // Capacidade da memória interna
    // int k = 3;  // Número de arquivos abertos
    int r = 3;  // Número de sequências iniciais

    char metodo = 'B';
    for (size_t k = 4; k <= 12; k += 2)
    {
        for (size_t m = 3; m <= 68; m += 15)
        {
            for (size_t i = 1; i <= 451; i+50)
            {
            
                    r = i;
                    vector<vector<int>> sequencias = gerarSequenciasIniciaisSimulacao(m, r);
                    vector<pair<int, float>> listBetas; // Recuperar valores de Beta para cada fase
                    int nRegistros = calcularQtdRegistros(sequencias);
                    int escritasArquivo = 0; // Para calcular alfa
                    pair<float, float> alfaBeta;

                    cout << fixed << setprecision(2);

                    criarArquivos(k);


                    switch (metodo) {
                        case 'B':
                            alfaBeta = ordenacaoBalanceada(sequencias, m, k, r, nRegistros, listBetas, escritasArquivo);
                            break;
                        case 'P':
                            // alfaBeta = ordenacaoPolifasica(sequencias, m, k, r, nRegistros);
                            break;
                        case 'C':
                            // alfaBeta = ordenacaoCascata(dados, m, k, r, nRegistros);
                            break;
                        default:
                            cerr << "Método de ordenação inválido!" << endl;
                            return 1;
                    
                    }

                    X.push_back(alfaBeta.first);
                    Y.push_back(alfaBeta.second);
                
            
            }
        }

    }

    plt::figure_size(1920, 1080);
    plt::plot(X, Y, "r-"); // "r-" indica uma linha vermelha
    // plt::savefig("grafico.png");
    plt::save("grafico.png");

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
