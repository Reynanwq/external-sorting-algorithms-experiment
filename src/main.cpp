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
pair<float, float> ordenacaoPolifasica(vector<vector<int>>& sequencias, int memoria, int arquivosAbertos, int maxSequenciasIniciais, int nRegistros);
pair<float, float> ordenacaoCascata(vector<int>& dados, int memoria, int arquivosAbertos, int maxSequenciasIniciais, int nRegistros);
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
    k = 4, r = 49; // temp
    // char metodo = 'B';
    char metodo = 'P'; // Pode ser 'B', 'P' ou 'C'

    switch (metodo) {
        case 'B':
            ordenacaoBalanceada(sequencias, m, k, r, nRegistros, listBetas, escritasArquivo);
            break;
        case 'P':
            ordenacaoPolifasica(sequencias, m, k, r, nRegistros);
            break;
        case 'C':
            ordenacaoCascata(dados, m, k, r, nRegistros);
            break;
        default:
            cerr << "Método de ordenação inválido!" << endl;
            return 1;
    }

    double alfa = calcularAlfa(escritasArquivo, nRegistros);
    cout << "final " << alfa << endl;
    
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
pair<float, float> ordenacaoPolifasica(vector<vector<int>>& sequencias, int memoria, int arquivosAbertos, int maxSequenciasIniciais, int nRegistros) {
    if (memoria){} // temp - avoid flags
    if (sequencias.empty()){} // temp - avoid flags
    int escritasArquivo = 0;
    float alfa = 0, beta = 0;

    // gerarSequenciasIniciais(dados, memoria, maxSequenciasIniciais);
    vector<int> divisaoSequencias = calcularDivisaoSequenciasIniciais(maxSequenciasIniciais, arquivosAbertos);
    salvarListasEmArquivosBinary(sequencias, arquivosAbertos, maxSequenciasIniciais);

    fs::path folder = criarPasta("pages");
    vector<fstream> arquivos = abrirArquivosDB(folder, arquivosAbertos);

    int arquivoVazio = qualArquivoEstaVazio(arquivos);
    cout << "Arquivo vazio: " << arquivoVazio << endl;

    // Próximos passos
    // chamar merge aqui, retornar o Beta 0
    // Criar função para salvar estado atual das páginas em uma estrutura de ValorBinary
    // Criar função para imprimir estrutura, ela recebe a fase atual, Beta e a estrutura com ValorBinary
    // Depois que as firulas passaram chama o merge real
    // Criar / chamar merge real 
        // Verifica qual é a página vazia
        // criar vector para ser utilizado ao final, nele se guarda o tamanho das sequencias de cada registro e põe para -1 a página vazia
        // Lê um valor de cada página não vazia e seta os seus endereços como não ocupado
        // Loop começa adicionando o menor valor ao arquivo vazio
        // Condição de parada - Criar função que calcula quantas páginas vazias existem - se for igual a qtdArquivos -1 -> para
            // O tamanho de sequencia desse registro será a soma do tamanho das sequencias dos n registros iniciais
            // Lê um valor da pagina do registro que foi armazenado e 
            // Verifica se o valor lido é da próxima sequência - if
                // caso seja é necessário retornar o ponteiro de leitura daquele arquivo em 1 posicao
            // Verifia também se não chegou no .eof para poder colocar na heap - else if
                // Se não chegou coloca no heap mínima e seta seu endereço no arquivo para não ocupado
            // Verifica se um dos três arquivos chegou no fim  | .eof - if
                // Caso sim - Criar função para Copiar os valores do final dos arquivos que não chegaram no fim para frente
                // chamar função para truncar arquivo que chegou ao final

    alfa = calcularAlfa(escritasArquivo, nRegistros);
    return make_pair(alfa, beta);
}

// Função para ordenação em cascata
pair<float, float> ordenacaoCascata(vector<int>& dados, int memoria, int arquivosAbertos, int maxSequenciasIniciais, int nRegistros) {
    if (arquivosAbertos == 0 || nRegistros == 0){}; // temp - avoid flags
    int escritasArquivo = 0;
    float alfa = 0, beta = 0;
    
    gerarSequenciasIniciais(dados, memoria, maxSequenciasIniciais);
    // Implementar a ordenação em cascata
    cout << "Ordenação em Cascata ainda não implementada." << endl;

    alfa = calcularAlfa(escritasArquivo, nRegistros);
    return make_pair(alfa, beta);
}
