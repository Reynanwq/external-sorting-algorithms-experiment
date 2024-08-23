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
#include <limits>

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

vector<fstream> criarArquivosDB(const fs::path& folder, const int& arquivosAbertos) {
    vector<fstream> arquivos(arquivosAbertos);
    for (int i = 0; i < arquivosAbertos; ++i) {
        fs::path filePath = folder / (to_string(i) + ".db");
        arquivos[i].open(filePath, ios::in | ios::out | ios::trunc | ios::binary);
        if (!arquivos[i].is_open()) {
            cerr << "Erro ao abrir o arquivo: " << filePath << endl;
            exit(-1);
        }
    }
    return arquivos;
}

vector<fstream> abrirArquivosDB(const fs::path& folder, const int& arquivosAbertos) {
    vector<fstream> arquivos(arquivosAbertos);
    for (int i = 0; i < arquivosAbertos; ++i) {
        fs::path filePath = folder / (to_string(i) + ".db");
        arquivos[i].open(filePath, ios::in | ios::out | ios::binary);
        if (!arquivos[i].is_open()) {
            cerr << "Erro ao abrir o arquivo: " << filePath << endl;
            exit(-1);
        }
    }
    return arquivos;
}

void fecharArquivosDB(vector<fstream>& arquivos) {
        for (auto& arquivo : arquivos) {
            arquivo.close();
        }
    }

fstream truncarArquivoDB(fstream& arquivoAberto, int& indiceArquivo) {
    arquivoAberto.close();
    fs::path folder = pegarPastaPages();
    fs::path filePath = folder / (to_string(indiceArquivo) + ".db");
    fstream arquivo;
    arquivo.open(filePath, ios::in | ios::out | ios::trunc | ios::binary);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filePath << endl;
        exit(-1);
    }
    return arquivo;
}

bool arquivoVazio(fstream& arquivo) {
    streampos posicao = arquivo.tellg();
    arquivo.seekg(0, ios::end);
    bool vazio = (arquivo.tellg() == 0);
    arquivo.seekg(posicao);
    return vazio;
}

int qualOPrimeiroArquivoVazio(vector<fstream>& arquivos) {
    for (unsigned long i = 0; i < arquivos.size(); i++) {
        if (arquivoVazio(arquivos[i]))
            return i;
    }
    return -1;
}

vector<int> quaisArquivosEstaoVazios(vector<fstream>& arquivos) {
    vector<int> indicesArquivosVazios;
    for (unsigned long i = 0; i < arquivos.size(); i++) {
        if (arquivoVazio(arquivos[i]))
            indicesArquivosVazios.push_back(i);
    }
    return indicesArquivosVazios;
}

int quantosArquivosCheios(vector<fstream>& arquivos) {
    int arquivosCheios = 0;

    for (unsigned long i = 0; i < arquivos.size(); i++) {
        if (!arquivoVazio(arquivos[i]))
            arquivosCheios++;
    }
    
    return arquivosCheios;
}

vector<vector<ValorBinary>> salvarArquivoNaMemoria(fstream& arquivo) {
    streampos posicao = arquivo.tellg();
    arquivo.seekg(0, ios::beg);
    int numSequenciaAnterior = 0, numSequenciaAtual = 0;
    ValorBinary registro;
    vector<ValorBinary> sequenciaTemporaria;
    vector<vector<ValorBinary>> arquivoEmMemoria;

    arquivo.flush();
    while (arquivo.read(reinterpret_cast<char*>(&registro), sizeof(ValorBinary))) {
        // cout << registro.valor << " "; // Debug
        numSequenciaAtual = registro.sequencia;
        if (numSequenciaAnterior != numSequenciaAtual) {
            if (!sequenciaTemporaria.empty()) {
                arquivoEmMemoria.push_back(sequenciaTemporaria);
                sequenciaTemporaria.clear();
            }
            numSequenciaAnterior = numSequenciaAtual;
        }
        sequenciaTemporaria.push_back(registro);
    }
    arquivoEmMemoria.push_back(sequenciaTemporaria);
    // cout << endl; // Debug
    limparBufferStream(arquivo);
    arquivo.seekg(posicao);
    return arquivoEmMemoria;
}

vector<vector<vector<ValorBinary>>> salvarEstadoDosArquivosNaMemoria(vector<fstream>& arquivos) {
    vector<vector<vector<ValorBinary>>> arquivosEmMemoria;
    vector<vector<ValorBinary>> arquivoTemporario;

    for (unsigned long i = 0; i < arquivos.size(); i++) {
        arquivoTemporario = salvarArquivoNaMemoria(arquivos[i]);
        arquivosEmMemoria.push_back(arquivoTemporario);
    }

    return arquivosEmMemoria;    
}

ValorBinary lerRegistroDoArquivo(fstream& arquivo, const int& sequencia) {
    cout << "Entrou na função lerRegistroDoArquivo\n";
    cout << "Sequencia passada: " << sequencia << endl;
    ValorBinary registro = criarValorBinaryInvalido();
    // arquivo.clear(); // Limpar estados de erro - o erro de leitura impossibilita escritas por exemplo
    arquivo.flush(); // Força a escrita de tudo que está no buffer
    arquivo.read(reinterpret_cast<char*>(&registro), sizeof(ValorBinary));

    streampos posicao = arquivo.tellp(); // TESTE
    cout << "Posição do ponteiro tellp: " << posicao << endl << endl;// TESTE
    
    if (!arquivo.eof() && sequencia != registro.sequencia) {
        retornarPonteiroDeLeituraUmRegistro(arquivo, registro);
        registro = criarValorBinaryInvalido();
        exibirValorBinary(registro);
    } else if (!arquivo.eof()) {
        exibirValorBinary(registro);
        escreverRegistroDesocupadoNoArquivo(arquivo, registro);
    } else if (arquivo.eof()){
        cout << "Chegou no final do arquivo\n";
    }
    // exibirValorBinary(registro);
    arquivo.clear();

    cout << "Saiu da função lerRegistroDoArquivo\n\n///////////////////////////////////////////////////\n\n"; // TESTE
    return registro;
}

ValorBinary criarValorBinaryInvalido() {
    ValorBinary registro = {false, true, numeric_limits<int>::max(), numeric_limits<int>::max(), 0, 0, 0};
    return registro; 
}

void retornarPonteiroDeLeituraUmRegistro(fstream& arquivo, const auto& registro) {
    arquivo.seekg(-1 * sizeof(registro), ios::cur);
}

int escreverRegistroNoArquivo(fstream& arquivo, ValorBinary& registro) {
    arquivoEstaAberto(arquivo);
    arquivo.clear();
    arquivo.write(reinterpret_cast<char*>(&registro), sizeof(ValorBinary));
    if (arquivo.good()) {
        return 1;
    } else {
        cerr << "Erro no armazenamento do registro" << endl;
        cout << "Erro na função escreverRegistroNoArquivo\n";
        exit(-1);
    }
}

int escreverRegistroDesocupadoNoArquivo(fstream& arquivo, const ValorBinary& registro) {
    cout << "Entrou na função escreverRegistroDesocupadoNoArquivo\n";
    arquivoEstaAberto(arquivo);
    streampos posicao = arquivo.tellp(); // TESTE
    cout << "Posição do ponteiro tellp: " << posicao << endl << endl;
    // if ()

    ValorBinary registroDesocupado = registro;
    registroDesocupado.ocupado = false;
    arquivo.clear();
    arquivo.write(reinterpret_cast<char*>(&registroDesocupado), sizeof(ValorBinary));
    if (arquivo.good()) {
        return 1;
    } else {
        cerr << "Erro no armazenamento do registro" << endl;
        cout << "Erro na função escreverRegistroDesocupadoNoArquivo\n";
        exit(-1);
    }
}

bool arquivoEstaAberto(const fstream& arquivo) {
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        exit(-1);
    }
    return true;
}

priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> lerRegistrosIniciais(vector<fstream>& arquivos, const int& memoria, const int& indiceArquivoVazio, vector<ValorBinary>& ultimoRegistroLidoDeCadaArquivo, const int& numSequencia) {
    priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> heapValorBinary;
    
    ValorBinary registro;
    int loops = 0, resto = 0;
    bool lerRegistro = true;
    vector<pair<int, int>> ponteirosIO(arquivos.size(), make_pair(0, 0)); // ISSO AQUI VAI TER QUE SER PASSADO POR REFERÊNCIA PELA FUNÇÃO QUE CHAMA ESTA FUNÇÃO
    // CRIAR ESTRUTURAS PARA AJUDAR A GUARDAR O ESTADO DE CADA PONTEIRO
    // TALVEZ UM VECTOR<PAIR DE INT INT> - FIRST É O GET E O SECOND O WRITE
    // PASSAR A INFORMAÇÃO DE QUAL O INDICE DO ARQUIVO QUE ESTÁ SENDO TRABALHADO, ALÉM DESSA ESTRUTURA

    // FUNÇÕES QUE PRECISAM MUDAR
    // lerRegistroDoArquivo
    // retornarPonteiroDeLeituraUmRegistro
    // escreverRegistroDesocupadoNoArquivo
    // quantosArquivosChegaramEOF

    int teste = 0; // TESTE

    streampos posicao = arquivos[0].tellp(); // TESTE
    cout << "Posição do ponteiro tellp: " << posicao << endl << endl; // TESTE

    while (lerRegistro) {
        cout << "Tamanho da pilha: " << heapValorBinary.size() << endl << endl; // TESTE
        resto = loops % arquivos.size();
        cout << "Resto registros iniciais: " << resto << endl;
        if (resto == indiceArquivoVazio || arquivos[resto].eof()) {
            loops++;
            continue;
        }
        cout << "Priemeira vez que vai entrar em lerRegistroDoArquivo\n"; // TESTE
        registro = lerRegistroDoArquivo(arquivos[resto], numSequencia);
        if (registro.sequencia != numSequencia && registro.ocupado == true){
            loops++;
            continue;
        }
        

        heapValorBinary.push(registro);

        ultimoRegistroLidoDeCadaArquivo[resto] = registro;        

        if (heapValorBinary.size() > static_cast<unsigned long>(memoria)){
            lerRegistro = false;
        } else {
            unsigned long contadorFinalArquivo = quantosArquivosChegaramEOF(arquivos, indiceArquivoVazio);
            if (contadorFinalArquivo == arquivos.size() - 1)
                lerRegistro = false;
        }

        if (teste == 1) // TESTE
        {
            exit(-1);
        }
        teste++;
        loops++;
    }

    return heapValorBinary;
}

int esvaziarFilaDePrioridade(priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary>& heap, fstream& arquivo, const ValorBinary& registro) {
    ValorBinary regInsert;
    int escritasEmArquivo = 0, i = 1;
    while (heap.size()) {
        regInsert = heap.top();
        heap.pop();
        regInsert.posicao = registro.posicao + i;
        regInsert.sequencia = registro.sequencia;
        regInsert.tamSequencia = registro.tamSequencia;
        regInsert.arquivoOrigem = registro.arquivoOrigem;
        escritasEmArquivo += escreverRegistroNoArquivo(arquivo, regInsert);
        i++;
    }
    return escritasEmArquivo;
}

int quantosArquivosChegaramEOF(vector<fstream>& arquivos, const int& indiceArquivoVazio) {
    int contadorFinalArquivo = 0;
    for (int i = 0; static_cast<unsigned long>(i) < arquivos.size(); i++) {
        if (i != indiceArquivoVazio) {
            if (arquivos[i].eof())
                contadorFinalArquivo++;
        }
    }
    return contadorFinalArquivo;
}

int qualArquivoChegouEmEOF(vector<fstream>& arquivos, const int& indiceArquivoVazio) {
    int indiceArquivoEOF = 0;
    for (int i = 0; static_cast<unsigned long>(i) < arquivos.size(); i++) {
        if (i != indiceArquivoVazio) {
            if (arquivos[i].eof())
                return indiceArquivoEOF;
        }
    }
    return -1;
}

int somarTamanhoSequencias(const vector<ValorBinary>& registros) {
    int tamanhoSequencias = 0;

    for (unsigned long i = 0; i < registros.size(); i++) {
        if (registros[i].ocupado == true && registros[i].registroFalso == false)
            tamanhoSequencias += registros[i].tamSequencia;
    }

    return tamanhoSequencias;
}

void limparBufferStreamVector(vector<fstream>& arquivos) {
    for (unsigned long i = 0; i < arquivos.size(); i++) {
        limparBufferStream(arquivos[i]);
    }
}

void limparBufferStream(fstream& arquivo) {
    arquivo.flush();
    arquivo.clear();
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

fs::path pegarPastaPages() {
    // Cria a pasta 'pages' caso ela não exista
    fs::path folder = "pages";
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }
    return folder;
}

// Imprimindo resultados
void imprimirResultadoValorBinary(const vector<vector<vector<ValorBinary>>>& listaDeMatriz, const int& fase, const double& beta) {
    cout << "fase " << fase << " " << beta << endl;
    for (unsigned long i = 0; i < listaDeMatriz.size(); i++) {
        if (listaDeMatriz[i][0].size() != 0){
            cout << i + 1 << ": ";
            for (unsigned long j = 0; j < listaDeMatriz[i].size(); j++) {
                for (unsigned long k = 0; k < listaDeMatriz[i][j].size(); k++) {
                    if (listaDeMatriz[i][j].size() == 1)
                        cout << "{" << listaDeMatriz[i][j][k].valor << "}";
                    else if (k == 0)
                        cout << "{" << listaDeMatriz[i][j][k].valor << " ";
                    else if (k != listaDeMatriz[i][j].size() - 1)
                        cout << listaDeMatriz[i][j][k].valor << " ";
                    else
                        cout << listaDeMatriz[i][j][k].valor << "}";
                    
                }
            }
            cout << endl;
        }
    }
}


// calculando para saída
int quantidadeDeRegistrosValorBinary(const vector<vector<vector<ValorBinary>>>& arquivosEmMemoria) {
    int quantidadeRegistros = 0;

    for (unsigned long i = 0; i < arquivosEmMemoria.size(); i++) {
        for (unsigned long j = 0; j < arquivosEmMemoria[i].size(); j++) {
            if (!arquivosEmMemoria[i][j].empty())
                quantidadeRegistros += arquivosEmMemoria[i][j][0].tamSequencia;
        }   
    }

    return quantidadeRegistros;
}

int quantidadeDeSequenciasValorBinary(const vector<vector<vector<ValorBinary>>>& arquivosEmMemoria) {
    int quantidadeSequencias = 0;

    for (unsigned long i = 0; i < arquivosEmMemoria.size(); i++) {
            if (arquivosEmMemoria[i].size() <= 1) {
                if (!arquivosEmMemoria[i][0].empty())
                    quantidadeSequencias += arquivosEmMemoria[i].size();
            } else {
                quantidadeSequencias += arquivosEmMemoria[i].size();
            }
    }

    return quantidadeSequencias;
}

// Heap para ValorBinary

