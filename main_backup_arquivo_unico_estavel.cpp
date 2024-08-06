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

using namespace std;
namespace fs = std::filesystem; // Alias para evitar digitar std::filesystem repetidamente

// Struct Element para heap mínima
struct Element {
    int value;
    bool marked;

    Element(int v, bool m = false) : value(v), marked(m) {}
};

// Função de comparação personalizada para Element
struct Compare {
    bool operator()(const Element& a, const Element& b) const {
        if (a.marked != b.marked) {
            return a.marked > b.marked; // elementos marcados são maiores
        }
        return a.value > b.value; // se ambos são marcados ou não marcados, compare os valores
    }
};

// Estrutura para armazenar o valor e sua origem - intercalação
struct Valor {
    int valor;
    int origem;
    int linha;
};

// Comparador para a fila de prioridade da intercalação
struct Comparador {
    bool operator()(const Valor& a, const Valor& b) {
        return a.valor > b.valor; // Para min-heap
    }
};

// Funções para os métodos de ordenação externa
void ordenacaoBalanceada(vector<vector<int>>& sequencias, int memoria, int arquivosTotais, int r, int nRegistros, vector<pair<int, float>> listBetas, int& escritasArquivo);
void ordenacaoPolifasica(vector<int>& dados, int m, int k, int r);
void ordenacaoCascata(vector<int>& dados, int m, int k, int r);

// Funções auxiliares
// void gerarSequenciasIniciais(vector<int>& dados, int m, int r);
vector<vector<int>> gerarSequenciasIniciais(vector<int> dados, int m, int r);

void intercalaSequencias(vector<int>& dados, int m, int k);
float intercalaSalvarCalcular(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas );
void intercalaSequenciasFaseUm(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo);
void intercalaSequenciasFaseDois(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo);
void calcularMetricas(const vector<int>& dados, int r, int k);

double calcularAlfa(int escritasArquivo, int nRegistros) ;
double calcularBeta(int memoria, int nSequencias, int nRegistros);
// Funções para processamento e geração de dados
vector<int> gerarDadosAleatorios(int n);
void exibirResultados(const vector<int>& dados, int r, int k);

// Funções para criação e processamento da heap mínima
void desmarcarTodosElementos(priority_queue<Element, vector<Element>, Compare>& heap);

// Funções para exibir
void exibirListaDeListasInt(vector<vector<int>>& listas);
void exibirListaInt(vector<int>& lista, string nome);
void exibirHeap(priority_queue<Element, vector<Element>, Compare> heap);


// Manipulando arquivos
void criarArquivos(int n);
// Cria ou limpa arquivos em um range
void criarArquivosRange(int inicio, int fim);
void salvarListasEmArquivos(const vector<vector<int>>& listas, int arquivosIntercalados);
bool apenasUmArquivoPreenchido(int n);
void salvarEstadoDasPaginas(int fase, int arquivosIntercalados, vector<vector<vector<int>>>& dadosEstado);

int calcularQtdRegistros(const vector<vector<int>>& sequencias);
int calcularNumeroSequencias(const vector<vector<vector<int>>>& sequencias);

void imprimirSaida(int fase, float beta, const vector<vector<vector<int>>>& sequencias);
void imprimirSaidaIncremento(int fase, float beta, const vector<vector<vector<int>>>& sequencias, int incrementoPaginas);

void mergeArquivosFaseUm(int& paginasMerge, int& escritasArquivo);
void mergeArquivosFaseDois(int paginasMerge, int& escritasArquivo);

int main() {
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
    char metodo = 'B'; // Pode ser 'B', 'P' ou 'C'

    switch (metodo) {
        case 'B':
            ordenacaoBalanceada(sequencias, m, k, r, nRegistros, listBetas, escritasArquivo);
            break;
        case 'P':
            ordenacaoPolifasica(dados, m, k, r);
            break;
        case 'C':
            ordenacaoCascata(dados, m, k, r);
            break;
        default:
            cerr << "Método de ordenação inválido!" << endl;
            return 1;
    }

    // intercalaSequencias(dados, m, k);
    // exibirResultados(dados, r, k);

    float alfa = calcularAlfa(escritasArquivo, nRegistros);
    cout << "final " << alfa << endl;
    
    return 0;
}

// Função para ordenação balanceada multi-caminhos
void ordenacaoBalanceada(vector<vector<int>>& sequencias, int memoria, int arquivosTotais, int r, int nRegistros, vector<pair<int, float>> listBetas, int& escritasArquivo) {
    int arquivosIntercalados =  floor(arquivosTotais/2);
    salvarListasEmArquivos(sequencias, arquivosIntercalados);
    int fase = 0;
    vector<vector<vector<int>>> estadoInicioFase;

    // intercalaSequenciasFaseUm(fase, arquivosIntercalados, estadoInicioFase);
    // fase++;        
    // intercalaSequenciasFaseDois(fase, arquivosIntercalados, estadoInicioFase);

    while (!apenasUmArquivoPreenchido(arquivosIntercalados)) {
        if (fase % arquivosIntercalados == 0) {
            intercalaSequenciasFaseUm(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas, escritasArquivo);
        } else {
            intercalaSequenciasFaseDois(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas, escritasArquivo);
        }
        // break;
        fase++;
    }

    float beta = intercalaSalvarCalcular(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas);
    if (fase % arquivosIntercalados == 0) {
        imprimirSaida(fase, beta, estadoInicioFase);
    } else {
        imprimirSaidaIncremento(fase, beta, estadoInicioFase, arquivosIntercalados);
    }
    // Implementar a ordenação balanceada multi-caminhos
    // intercalaSequenciasFaseUm(fase, arquivosIntercalados, estadoInicioFase);
    // cout << "Ordenação Balanceada Multi-Caminhos ainda não implementada." << endl;
}

// Função para ordenação polifásica
void ordenacaoPolifasica(vector<int>& dados, int m, int k, int r) {
    gerarSequenciasIniciais(dados, m, r);
    // Implementar a ordenação polifásica
    cout << "Ordenação Polifásica ainda não implementada." << endl;
}

// Função para ordenação em cascata
void ordenacaoCascata(vector<int>& dados, int m, int k, int r) {
    gerarSequenciasIniciais(dados, m, r);
    // Implementar a ordenação em cascata
    cout << "Ordenação em Cascata ainda não implementada." << endl;
}

vector<vector<int>> gerarSequenciasIniciais(vector<int> dados, int m, int r) {
    // priority_queue<int, vector<int>, greater<int>> heap;
    priority_queue<Element, vector<Element>, Compare> heap;
    vector<vector<int>> sequencias;
    vector<int> sequenciaAtual;
    int ultimoPush = 0;

    // vector<int> dados = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};

    while (!dados.empty() || !heap.empty() || !sequenciaAtual.empty()) {
        if (heap.size() < m && !dados.empty()) {
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
    while (sequencias.size() < m) {
        sequencias.push_back({});
    }

    // Verificar se a quantidade de sequências iniciais é igual r
    if (sequencias.size() > r){
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

// Função para calcular métricas
void calcularMetricas(const vector<int>& dados, int r, int k) {
    int totalOperacoesEscrita = 0; // Placeholder
    int totalRegistros = dados.size();
    double alpha = static_cast<double>(totalOperacoesEscrita) / totalRegistros;
    cout << "α(r): " << alpha << endl;
    cout << "Cálculo de Métricas ainda não implementado." << endl;
}

// Função para calcular alfa
double calcularAlfa(int escritasArquivo, int nRegistros) {
    return round((escritasArquivo / nRegistros) * 100) / 100;
}

// Função para calcular beta
double calcularBeta(int memoria, int nSequencias, int nRegistros){
    return round((1.0 / (memoria * nSequencias)) * nRegistros * 100) / 100;
}

// Função para gerar dados aleatórios
vector<int> gerarDadosAleatorios(int n) {
    vector<int> dados(n);
    srand(static_cast<unsigned>(time(0))); // Semente para números aleatórios
    for (int& valor : dados) {
        valor = rand() % 100; // Números aleatórios entre 0 e 99
    }
    return dados;
}

// Função para exibir resultados
void exibirResultados(const vector<int>& dados, int r, int k) {
    cout << "Dados ordenados: ";
    for (int valor : dados) {
        cout << valor << " ";
    }
    cout << endl;
    // Implementar exibição de métricas e fases
    cout << "Resultados ainda não implementados." << endl;
}

// Função para desmarcar todos os elementos
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

// Funções para exibir
void exibirListaDeListasInt(vector<vector<int>>& listas) {
    int contador = 0;
    for (vector<int> lista : listas) {
        cout << contador << ": { ";
        for (int valor : lista) {
            cout << valor << " ";
        }
        cout << " }" << endl;
        contador++;
    }
    cout << "-----------------------------------------" << endl << endl << endl;
}

void exibirListaInt(vector<int>& lista, string nome) {
    cout <<  nome << ": ";
    for (int valor : lista ) {
        cout << valor << " ";
    }
    cout << endl;
}

void exibirHeap(priority_queue<Element, vector<Element>, Compare> heap) {
    cout << "Heap: ";
    while (!heap.empty())
    {
        if (!heap.top().marked)
            cout << heap.top().value << " ";
        else
            cout << heap.top().value << "* ";
        
        heap.pop();
    }
    cout << endl;
}

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

// Cria ou limpa arquivos em um range
void criarArquivosRange(int inicio, int fim) {
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

void salvarListasEmArquivos(const vector<vector<int>>& listas, int arquivosIntercalados) {
    // Cria a pasta 'pages' caso ela não exista
    fs::path folder = "pages";
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }

    // Abrir os arquivos de saída
    vector<ofstream> arquivos(arquivosIntercalados);
    for (int i = 0; i < arquivosIntercalados; ++i) {
        fs::path filePath = folder / (std::to_string(i) + ".txt");
        arquivos[i].open(filePath, ios::app); // Abre o arquivo em modo append
        if (!arquivos[i].is_open()) {
            cerr << "Erro ao abrir o arquivo: " << filePath << endl;
            return;
        }
    }

    // Salva as listas nos arquivos correspondentes
    for (size_t i = 0; i < listas.size(); ++i) {
        int arquivoIndex = i % arquivosIntercalados;
        int tamanhoLista = listas[i].size();
        int contador = 0;
        for (const int& valor : listas[i]) {
            if (contador < tamanhoLista - 1) {
                arquivos[arquivoIndex] << valor << " ";
            } else {
                arquivos[arquivoIndex] << valor;
            }
            contador++;
        }
        arquivos[arquivoIndex] << endl;
    }

    // Fecha os arquivos
    for (int i = 0; i < arquivosIntercalados; ++i) {
        if (arquivos[i].is_open()) {
            arquivos[i].close();
        }
    }
}

float intercalaSalvarCalcular(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas ) {
    salvarEstadoDasPaginas(fase, arquivosIntercalados, estadoInicioFase);
    int nSequencias = calcularNumeroSequencias(estadoInicioFase);
    float beta = calcularBeta(memoria, nSequencias, nRegistros);
    listBetas.push_back(make_pair(fase, beta)); // Salvando estado de beta para calcular desempenho
    return beta;
}

void intercalaSequenciasFaseUm(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo) {
    float beta = intercalaSalvarCalcular(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas);
    imprimirSaida(fase, beta, estadoInicioFase);
    mergeArquivosFaseUm(arquivosIntercalados, escritasArquivo);
    criarArquivosRange(0, arquivosIntercalados); // Apaga conteúdo antigo
}

void intercalaSequenciasFaseDois(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo) {
    float beta = intercalaSalvarCalcular(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas);
    imprimirSaidaIncremento(fase, beta, estadoInicioFase, arquivosIntercalados);
    mergeArquivosFaseDois(arquivosIntercalados, escritasArquivo);
    criarArquivosRange(arquivosIntercalados, arquivosIntercalados * 2); // Apaga conteúdo antigo
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

void salvarEstadoDasPaginas(int fase, int arquivosIntercalados, vector<vector<vector<int>>>& dadosEstado) {
    int turno = fase % 2;
    int inicio, fim;

    if (turno == 0) {
        inicio = 0;
        fim = arquivosIntercalados;
    } else {
        inicio = arquivosIntercalados;
        fim = arquivosIntercalados * 2;
    }

    // Limpa dadosEstado para evitar dados antigos
    dadosEstado.clear();
    
    // Lê os arquivos de acordo com a faixa
    for (int i = inicio; i < fim; ++i) {
        fs::path filePath = "pages/" + to_string(i) + ".txt";
        ifstream arquivo(filePath);
        if (!arquivo.is_open()) {
            cerr << "Não foi possível abrir o arquivo: " << i << ".txt" << endl;
            continue;
        }

        vector<vector<int>> matrizArquivo;
        string linha;
        while (getline(arquivo, linha)) {
            vector<int> linhaDados;
            size_t pos = 0;
            while ((pos = linha.find(' ')) != string::npos) {
                string token = linha.substr(0, pos);
                if (!token.empty()) {
                    linhaDados.push_back(stoi(token));
                }
                linha.erase(0, pos + 1);
            }
            if (!linha.empty()) {
                linhaDados.push_back(stoi(linha)); // Adiciona o último número se não estiver vazio
            }
            matrizArquivo.push_back(linhaDados);
        }
        arquivo.close();

        // Adiciona a matriz do arquivo ao dadosEstado
        dadosEstado.push_back(matrizArquivo);
    }
}

int calcularQtdRegistros(const vector<vector<int>>& sequencias){
    int qtdRegistro = 0;
    for (int i = 0; i < sequencias.size(); i++) {
        qtdRegistro += sequencias[i].size();
    }
    return qtdRegistro;
}

int calcularNumeroSequencias(const vector<vector<vector<int>>>& sequencias){
    int qtdSequencias = 0;
    for (int i = 0; i < sequencias.size(); i++) {
        qtdSequencias += sequencias[i].size();
    }
    return qtdSequencias;
}

void imprimirSaida(int fase, float beta, const vector<vector<vector<int>>>& sequencias){
    cout << "fase " << fase << " " << beta << endl;
    for (int i = 0; i < sequencias.size(); i++) {
        if (sequencias[i].size()) {
            cout << i + 1 << ": ";
            for (int j = 0; j < sequencias[i].size(); j++){
                for (int k = 0; k < sequencias[i][j].size(); k++) {
                    if (k == 0){
                        cout << "{" << sequencias[i][j][k] << " ";
                    } else if(k < sequencias[i][j].size() - 1) {
                        cout << sequencias[i][j][k] << " ";
                    } else {
                        cout << sequencias[i][j][k] << "}";
                    }
                }
            }
            cout << endl;

        }
    }
}

void imprimirSaidaIncremento(int fase, float beta, const vector<vector<vector<int>>>& sequencias, int incrementoPaginas){
    cout << "fase " << fase << " " << beta << endl;
    for (int i = 0; i < sequencias.size(); i++) {
        if (sequencias[i].size()) {
            cout << i + 1 + incrementoPaginas << ": ";
            for (int j = 0; j < sequencias[i].size(); j++){
                for (int k = 0; k < sequencias[i][j].size(); k++) {
                    if (k == 0){
                        cout << "{" << sequencias[i][j][k] << " ";
                    } else if(k < sequencias[i][j].size() - 1) {
                        cout << sequencias[i][j][k] << " ";
                    } else {
                        cout << sequencias[i][j][k] << "}";
                    }
                }
            }
            cout << endl;
        }
    }
}

void mergeArquivosFaseUm(int& paginasMerge, int& escritasArquivo) {
    fs::path folder = "pages";
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }

    int linhaAtual = 0;
    vector<bool> finalLinha, finalArquivo;
    vector<ifstream> arquivos(paginasMerge);
    priority_queue<Valor, vector<Valor>, Comparador> minHeap;

    for (int i = 0; i < paginasMerge; i++) {
        finalLinha.push_back(false);
        finalArquivo.push_back(false);
    }
    
    criarArquivosRange(paginasMerge, paginasMerge * 2);

    // Abrir arquivos para leitura
    for (int i = 0; i < paginasMerge; ++i) {
        fs::path nomeArquivo = folder / (to_string(i) + ".txt");
        arquivos[i].open(nomeArquivo);
        if (!arquivos[i].is_open()) {
            cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
            return;
        }
    }

    // Função para ler todos os valores da linha atual de um arquivo específico
    auto lerValorLinha = [&](int indiceArquivo, int linha) -> bool {
        int valor;
        bool leuAlgo = false;

        // verifica se o final de linha ou de arquivo foi alcançado anteriormete
        if (finalLinha[indiceArquivo] || finalArquivo[indiceArquivo])
            return leuAlgo;

        // Verifica se o próximo token é o final de linha ('\n')
        if (arquivos[indiceArquivo].peek() == '\n') {
            finalLinha[indiceArquivo] = true;
            // std::cout << "Quebra LINHA\n";
            return leuAlgo;
        }

        // Verifica se o próximo token é o final de arquivo (EOF)
        if (arquivos[indiceArquivo].peek() == EOF) {
            finalArquivo[indiceArquivo] = true;
            // std::cout << "Fim arquivo\n";
            return leuAlgo;
        }

        // Lê o próximo valor válido
        if (arquivos[indiceArquivo] >> valor) {
            minHeap.push({valor, indiceArquivo, linha});
            leuAlgo = true;
        } else {
            // std::cout << "Não leu valor válido\n";
        }
        return leuAlgo;
    };

    // Inicializar a heap com o primeiro valor de cada arquivo
    for (int i = 0; i < paginasMerge; ++i) {
        lerValorLinha(i, linhaAtual);
    }

    bool leituraAtiva = true;

    while (leituraAtiva) {
        leituraAtiva = false;
        // std::cout << "Leitura ativa: true" << endl;
        // Processar todos os valores da linha atual
        while (!minHeap.empty()) {
            Valor menor = minHeap.top();
            minHeap.pop();
            // std::cout << menor.valor << " " << menor.origem << " " << menor.linha << endl;
            int resto = linhaAtual % paginasMerge;
            fs::path nomeArquivoSaida = folder / (to_string(resto + paginasMerge) + ".txt");
            // std::cout << resto << " = " << linhaAtual << " % " << paginasMerge << endl;
            // std::cout << "pages/" << to_string(resto + paginasMerge) << ".txt" << endl << endl;
            ofstream arquivoSaida(nomeArquivoSaida, ios_base::app);
            if (!arquivoSaida.is_open()) {
                cerr << "Erro ao abrir o arquivo de saída " << nomeArquivoSaida << endl;
                return;
            }
            arquivoSaida << menor.valor;
            escritasArquivo++;
            bool leuValor = lerValorLinha(menor.origem, menor.linha);
            if (leuValor)
            {
                // std::cout << "LEU\n";
            } else {
                // std::cout << "Não LEU\n";
            }
            
            // Se a pilha está vazia add endl se não add " "
            if (minHeap.empty()) {
                // std::cout << "Add endl\n";
                arquivoSaida << endl;
                arquivoSaida.close();
            } else {
                arquivoSaida << " ";
            }
        }

        // Verificar se há mais linhas a serem lidas
        // bool finalLinhaBool = false;
        linhaAtual++;
        // std::cout << "linha Atual: " << linhaAtual << endl;
        for (int i = 0; i < paginasMerge; ++i) {
            if (!arquivos[i].eof()) { // !finalArquivo[i]
            // std::cout << "Ainda tem um arquivo com linha para ler: " << i << endl;
                leituraAtiva = true;
                if (finalLinha[i]){
                    finalLinha[i] = false;
                    int valor = 0;
                    if (arquivos[i] >> valor) {
                        minHeap.push({valor, i, linhaAtual});
                        // std::cout << "adicionou na pilha fora da função.\n";
                    }
                }
            }
        }

    }

    // Fechar arquivos
    for (int i = 0; i < paginasMerge; ++i) {
        arquivos[i].close();
    }
}

void mergeArquivosFaseDois(int paginasMerge, int& escritasArquivo) {
    fs::path folder = "pages";
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }

    int linhaAtual = 0;
    vector<bool> finalLinha, finalArquivo;
    vector<ifstream> arquivos(paginasMerge);
    priority_queue<Valor, vector<Valor>, Comparador> minHeap;

    for (int i = 0; i < paginasMerge; i++) {
        finalLinha.push_back(false);
        finalArquivo.push_back(false);
    }
    
    // Controle de onde escrever
    const int oInicio = 0;
    const int oFim = paginasMerge;

    // Controle de onde ler
    const int iInicio = paginasMerge;
    const int iFim = paginasMerge * 2;

    // Limpando arquivos de saída
    criarArquivosRange(oInicio, oFim);

    // Abrir arquivos para leitura
    for (int i = iInicio; i < iFim; ++i) {
        // cout << "Abrir arquivo " << i << endl;
        fs::path nomeArquivo = folder / (to_string(i) + ".txt");
        arquivos[i - paginasMerge].open(nomeArquivo);
        if (!arquivos[i - paginasMerge].is_open()) {
            cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
            return;
        }
    }

    // Função para ler todos os valores da linha atual de um arquivo específico
    auto lerValorLinha = [&](int indiceArquivo, int linha) -> bool {
        int valor;
        bool leuAlgo = false;

        // verifica se o final de linha ou de arquivo foi alcançado anteriormete
        if (finalLinha[indiceArquivo] || finalArquivo[indiceArquivo])
            return leuAlgo;

        // Verifica se o próximo token é o final de linha ('\n')
        if (arquivos[indiceArquivo].peek() == '\n') {
            finalLinha[indiceArquivo] = true;
            // std::cout << "Quebra LINHA\n";
            return leuAlgo;
        }

        // Verifica se o próximo token é o final de arquivo (EOF)
        if (arquivos[indiceArquivo].peek() == EOF) {
            finalArquivo[indiceArquivo] = true;
            // std::cout << "Fim arquivo\n";
            return leuAlgo;
        }

        // Lê o próximo valor válido
        if (arquivos[indiceArquivo] >> valor) {
            minHeap.push({valor, indiceArquivo, linha});
            leuAlgo = true;
        } else {
            // std::cout << "Não leu valor válido\n";
        }
        return leuAlgo;
    };

    // Inicializar a heap com o primeiro valor de cada arquivo
    for (int i = 0; i < paginasMerge; ++i) {
        lerValorLinha(i, linhaAtual);
    }

    bool leituraAtiva = true;

    while (leituraAtiva) {
        leituraAtiva = false;
        // std::cout << "Leitura ativa: true" << endl;
        // Processar todos os valores da linha atual
        while (!minHeap.empty()) {
            Valor menor = minHeap.top();
            minHeap.pop();
            // std::cout << menor.valor << " " << menor.origem << " " << menor.linha << endl;
            int resto = linhaAtual % paginasMerge;
            fs::path nomeArquivoSaida = folder / (to_string(resto) + ".txt");
            // std::cout << resto << " = " << linhaAtual << " % " << paginasMerge << endl;
            // std::cout << "pages/" << to_string(resto + paginasMerge) << ".txt" << endl << endl;
            ofstream arquivoSaida(nomeArquivoSaida, ios_base::app);
            if (!arquivoSaida.is_open()) {
                cerr << "Erro ao abrir o arquivo de saída " << nomeArquivoSaida << endl;
                return;
            }
            arquivoSaida << menor.valor;
            escritasArquivo++;
            bool leuValor = lerValorLinha(menor.origem, menor.linha);
            if (leuValor)
            {
                // std::cout << "LEU\n";
            } else {
                // std::cout << "Não LEU\n";
            }
            
            // Se a pilha está vazia add endl se não add " "
            if (minHeap.empty()) {
                // std::cout << "Add endl\n";
                arquivoSaida << endl;
                arquivoSaida.close();
            } else {
                arquivoSaida << " ";
            }
        }

        // Verificar se há mais linhas a serem lidas
        // bool finalLinhaBool = false;
        linhaAtual++;
        // std::cout << "linha Atual: " << linhaAtual << endl;
        for (int i = 0; i < paginasMerge; ++i) {
            if (!arquivos[i].eof()) { // !finalArquivo[i]
            // std::cout << "Ainda tem um arquivo com linha para ler: " << i << endl;
                leituraAtiva = true;
                if (finalLinha[i]){
                    finalLinha[i] = false;
                    int valor = 0;
                    if (arquivos[i] >> valor) {
                        minHeap.push({valor, i, linhaAtual});
                        // std::cout << "adicionou na pilha fora da função.\n";
                    }
                }
            }
        }

    }

    // Fechar arquivos
    for (int i = 0; i < paginasMerge; ++i) {
        arquivos[i].close();
    }
}