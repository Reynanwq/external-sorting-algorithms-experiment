// funcoes_gerais.cpp
#include "funcoes_gerais.h"
#include "funcoes_secundarias.h"
#include <iostream>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;
namespace fs = std::filesystem;

// teste
void minhaFuncaoGeral() {
    cout << "Minha função Geral foi chamada!" << endl;
}

// Reynan - Funções em uso
vector<int> gerarDadosAleatorios(int n) {
    vector<int> dados(n);
    srand(static_cast<unsigned>(time(0))); // Semente para números aleatórios
    for (int& valor : dados) {
        valor = rand() % 100; // Números aleatórios entre 0 e 99
    }
    return dados;
}

// Reynan - Funções não utilizadas ainda
void intercalaSequencias(vector<int>& dados, int m, int k) {
    if (k == 0){} // temp - avoid flags

    // Estrutura de dados para armazenar sequências iniciais
    vector<vector<int>> sequencias;
    gerarSequenciasIniciais(dados, m, sequencias.size());

    // Fila de prioridade para armazenar os elementos e suas origens
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Vetor de índices para manter o acompanhamento da posição de cada sequência
    vector<int> indices(sequencias.size(), 0);

    // Inicializar a fila de prioridade com o primeiro elemento de cada sequência
    for (unsigned long i = 0UL; i < sequencias.size(); ++i) {
        if (!sequencias[i].empty()) {
            pq.push(make_pair(sequencias[i][0], i));
        }
    }

    vector<int> dadosOrdenados;

    // Processo de intercalação
    while (!pq.empty()) {
        pair<int, int> p = pq.top();
        int valor = p.first;
        int origem = p.second;
        pq.pop();
        dadosOrdenados.push_back(valor);

        // Inserir o próximo elemento da sequência de origem, se existir
        if (static_cast<unsigned long>(++indices[origem]) < sequencias[origem].size()) {
            pq.push(make_pair(sequencias[origem][indices[origem]], origem));
        }
    }

    // Copiar os dados ordenados de volta para o vetor de dados original
    dados = dadosOrdenados;
}

void calcularMetricas(const vector<int>& dados, int r, int k) {
    if (r == 0 || k == 0){} // temp - avoid flags

    int totalOperacoesEscrita = 0; // Placeholder
    int totalRegistros = dados.size();
    double alpha = static_cast<double>(totalOperacoesEscrita) / totalRegistros;
    cout << "α(r): " << alpha << endl;
    cout << "Cálculo de Métricas ainda não implementado." << endl;
}

void exibirResultados(const vector<int>& dados, int r, int k) {
    if (r == 0 || k == 0){} // temp - avoid flags

    cout << "Dados ordenados: ";
    for (int valor : dados) {
        cout << valor << " ";
    }
    cout << endl;
    // Implementar exibição de métricas e fases
    cout << "Resultados ainda não implementados." << endl;
}

// Calculo desempenho
double calcularAlfa(int escritasArquivo, int nRegistros) {
    return round((escritasArquivo / nRegistros) * 100) / 100;
}

double calcularBeta(int memoria, int nSequencias, int nRegistros){
    return round((1.0 / (memoria * nSequencias)) * nRegistros * 100) / 100;
}

int calcularQtdRegistros(const vector<vector<int>>& sequencias){
    int qtdRegistro = 0;
    for (unsigned long i = 0; i < sequencias.size(); i++) {
        qtdRegistro += sequencias[i].size();
    }
    return qtdRegistro;
}

int calcularNumeroSequencias(const vector<vector<vector<int>>>& sequencias){
    int qtdSequencias = 0;
    for (unsigned long i = 0; i < sequencias.size(); i++) {
        qtdSequencias += sequencias[i].size();
    }
    return qtdSequencias;
}

// Utilizando heap mínima
vector<vector<int>> gerarSequenciasIniciais(vector<int> dados, int m, int r) {
    priority_queue<Element, vector<Element>, Compare> heap;
    vector<vector<int>> sequencias;
    vector<int> sequenciaAtual;
    int ultimoPush = 0;

    while (!dados.empty() || !heap.empty() || !sequenciaAtual.empty()) {
        if (heap.size() < static_cast<unsigned long>(m) && !dados.empty()) {
            // cout << "ADD HEAP" << endl;
            if (ultimoPush < dados.front()) {
                heap.push(Element(dados.front()));
            } else {
                heap.push(Element(dados.front(), true));
            }
            dados.erase(dados.begin());

        } else {
            // cout << "ADD SEQUENCIA" << endl;
            if (!heap.empty() && !heap.top().marked)  {
                sequenciaAtual.push_back(heap.top().value);
                ultimoPush = heap.top().value;
                heap.pop();

            } else {
                sequencias.push_back(sequenciaAtual);
                sequenciaAtual.clear();
                ultimoPush = 0;
                desmarcarTodosElementos(heap);
            }
        }
            // exibirListaInt(dados, "Registros");
            // exibirHeap(heap);
            // exibirListaInt(sequenciaAtual, "Sequencia Atual");
            // cout << "Heap mínima: " << heap.top().value << endl;
            // cout << "Marcado: " << heap.top().marked << endl;
            // exibirListaDeListasInt(sequencias);
    }

    // Garantir que geramos pelo menos m sequências
    while (sequencias.size() < static_cast<unsigned long>(m)) {
        sequencias.push_back({});
    }

    // Verificar se a quantidade de sequências iniciais é igual r
    if (sequencias.size() > static_cast<unsigned long>(r)){
        vector<vector<int>> primeirasRListas;
        for (int i = 0; i < r; i++)
        {
            primeirasRListas.push_back(sequencias[i]);
        }
        // Exibir sequências geradas para depuração
        // for (int i = 0; i < primeirasRListas.size(); ++i) {
        //     cout << "Sequência " << i + 1 << ": ";
        //     for (int valor : primeirasRListas[i]) {
        //         cout << valor << " ";
        //     }
        //     cout << endl;
        // }
        return primeirasRListas;
    }

    return sequencias;
}

void desmarcarTodosElementos(priority_queue<Element, vector<Element>, Compare>& heap) {
    vector<Element> temp;
    while (!heap.empty()) {
        Element elem = heap.top();
        heap.pop();
        elem.marked = false;
        temp.push_back(elem);
    }
    for (const Element& elem : temp) {
        heap.push(elem);
    }
}

// Manipulando arquivos
void criarArquivos(int n) {
    // namespace fs = std::filesystem; // Alias para std::filesystem
    fs::path folder = "pages";

    // Cria a pasta 'pages' caso ela não exista
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }

    // Cria n-1 arquivos .txt na pasta 'pages'
    for (int i = 0; i < n; ++i) {
        fs::path filePath = folder / (to_string(i) + ".txt");

        // Verifica se o arquivo já existe
        ofstream file(filePath, std::ios::trunc);
        if (file.is_open()) {
            file.close();
            // cout << "Arquivo criado: " << filePath << endl;
        } else {
            // cerr << "Erro ao criar o arquivo: " << filePath << endl;
        }
    }
}

vector<fstream> criarArquivosDB(const fs::path& folder, const int& arquivosAbertos) {
    vector<fstream> arquivos(arquivosAbertos);
    for (int i = 0; i < arquivosAbertos; ++i) {
        fs::path filePath = folder / (to_string(i) + ".db");
        arquivos[i].open(filePath, ios::in | ios::out | ios::trunc);
        if (!arquivos[i].is_open()) {
            cerr << "Erro ao abrir o arquivo: " << filePath << endl;
            return;
        }
    }
    return arquivos;
}

vector<fstream> abrirArquivosDB(const fs::path& folder, const int& arquivosAbertos) {
    vector<fstream> arquivos(arquivosAbertos);
    for (int i = 0; i < arquivosAbertos; ++i) {
        fs::path filePath = folder / (to_string(i) + ".db");
        arquivos[i].open(filePath, ios::in | ios::out | ios::app);
        if (!arquivos[i].is_open()) {
            cerr << "Erro ao abrir o arquivo: " << filePath << endl;
            return;
        }
    }
    return arquivos;
}

void fecharArquivosDB(vector<fstream>& arquivos) {
        for (auto& arquivo : arquivos) {
            arquivo.close();
        }
    }

void criarLimparArquivosRange(int inicio, int fim) {
    // namespace fs = std::filesystem; // Alias para std::filesystem
    fs::path folder = "pages";

    // Cria a pasta 'pages' caso ela não exista
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }

    // Cria fim-1 arquivos .txt na pasta 'pages'
    for (int i = inicio; i < fim; ++i) {
        fs::path filePath = folder / (to_string(i) + ".txt");

        // Verifica se o arquivo já existe
        ofstream file(filePath, std::ios::trunc);
        if (file.is_open()) {
            file.close();
            // cout << "Arquivo criado: " << filePath << endl;
        } else {
            // cerr << "Erro ao criar o arquivo: " << filePath << endl;
        }
    }
}

bool apenasUmArquivoPreenchido(int n) {
    fs::path folder = "pages";
    int countPreenchidos = 0;

    for (int i = 0; i < n; ++i) {
        fs::path filePath = folder / (to_string(i) + ".txt");
        if (fs::exists(filePath)) {
            ifstream file(filePath);
            string line;
            if (getline(file, line)) {
                if (!line.empty()) {
                    countPreenchidos++;
                    if (countPreenchidos > 1) {
                        return false;
                    }
                }
            }
            file.close();
        }
    }

    return countPreenchidos == 1;
}

// Manipulando pastas
fs::path criarPasta(string nomePasta) {
    // Cria a pasta 'pages' caso ela não exista
    fs::path folder = nomePasta;
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }
    return folder;
}

