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
// #include <limits>
#include <array>
#include <unistd.h>
#include <fcntl.h>

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

vector<vector<int>> gerarSequenciasIniciaisSimulacao(int m, int r) {
    priority_queue<Element, vector<Element>, Compare> heap;
    vector<vector<int>> sequencias;
    vector<int> sequenciaAtual;
    vector<int> dados = gerarDadosAleatorios(100*r);
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
        if (dados.empty() && sequencias.size() < static_cast<unsigned long>(r)) {
            dados = gerarDadosAleatorios(100*r);
        }
        
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

fstream abrirArquivoDB(const fs::path& folder, const int& indiceArquivo) {
    fstream arquivo;
    fs::path filePath = folder / (to_string(indiceArquivo) + ".db");

    arquivo.open(filePath, ios::in | ios::out | ios::binary);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filePath << endl;
        exit(-1);
    }

    return arquivo;
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
    // cout << "Pos Arquivos antes de salvar: " << posicao << endl; // TESTE
    arquivo.seekg(0, ios::beg);
    int numSequenciaAnterior = 0, numSequenciaAtual = 0;
    ValorBinary registro;
    vector<ValorBinary> sequenciaTemporaria;
    vector<vector<ValorBinary>> arquivoEmMemoria;

    arquivo.flush();
    while (arquivo.read(reinterpret_cast<char*>(&registro), sizeof(ValorBinary))) {
        // cout << registro.valor << " "; // TESTE
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

    // TESTE
    // if (arquivo.eof()) {
    //     cout << "\nFim do arquivo - EOF\n\n";
    // }

    arquivoEmMemoria.push_back(sequenciaTemporaria);
    // cout << "Fim" << endl; // TESTE
    limparBufferStream(arquivo); // TESTE_DEFINITIVO
    // TESTE_BUFFER
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

ValorBinary lerRegistroDoArquivo(fstream& arquivo, ArquivoIO& arquivoIO) {
    ValorBinary registro = ValorBinary::criarInvalido();
    if (arquivoIO.eof == 1)
    {
        cout << "EOF Abaixo\n";
        cout << "Lendo: " << arquivoIO.read << "\nOrigem: " << arquivoIO.origem << endl << endl;
        return registro;
    }
    
    arquivo.clear();
    cout << "Lendo: " << arquivoIO.read << "\nOrigem: " << arquivoIO.origem << endl << endl;
    arquivo.seekg(arquivoIO.read * sizeof(ValorBinary), ios::beg);
    arquivo.flush(); // Força a escrita de tudo que está no buffer
    arquivo.read(reinterpret_cast<char*>(&registro), sizeof(ValorBinary));

    if (arquivo.eof()){
        // cout << "Chegou em EOF\n\n";
        arquivoIO.choqueSequencias = true;
        arquivoIO.eof = true;
        // arquivoEstaEstavel(arquivo); // TESTE_REWRITE
        registro = ValorBinary::criarInvalido();
        arquivo.clear();
    } else if (registro.is_valido() && arquivoIO.sequencia == registro.sequencia) {
        arquivoIO.read++;
        escreverRegistroDesocupadoNoArquivo(arquivo, registro, arquivoIO);
    } else if (registro.is_valido() && arquivoIO.sequencia + 1 == registro.sequencia) {
        arquivoIO.choqueSequencias = true;
        registro = ValorBinary::criarInvalido();
    }
    cout << "Lendo: " << arquivoIO.read << "\nOrigem: " << arquivoIO.origem << "\nEscrita: " << arquivoIO.write << endl;
    cout << "Saída da função acima\n\n";
    // registro.exibir();
    return registro;
}

bool arquivoEstaEstavel(const fstream& arquivo) {
    if (!arquivo.good()) {
        if (arquivo.fail())
            cerr << "Falha na leitura (problema de formatação ou operação inválida)." << endl;
        else if (arquivo.bad())
            cerr << "Erro grave no stream." << endl;

        exit(-1); // TESTE
        return false;
    }

    return true;
}

// TESTE_REWRITE
void retornarPonteiroDeLeituraUmRegistro(fstream& arquivo, const auto& registro) {
    arquivo.seekg(-1 * sizeof(registro), ios::cur);
}

bool escreverRegistroNoArquivo(fstream& arquivo, ValorBinary& registro, ArquivoIO& arquivoIO) {
    // arquivoEstaAberto(arquivo);
    arquivoEstaEstavel(arquivo);
    // arquivoIO.exibir(); // TESTE_MUDANÇA_IMEDIATA
    // cout << "Início do escreverRegistroNoArquivo\n\n";
    arquivo.clear();
    arquivo.seekp(arquivoIO.write * sizeof(ValorBinary), ios::beg);
    arquivo.write(reinterpret_cast<char*>(&registro), sizeof(ValorBinary));
    if (arquivoEstaEstavel(arquivo)) {
        arquivoIO.write++;
        arquivo.flush();
        return true; 
    }
    
    return false;
}

bool escreverRegistroNoArquivoCopy(fstream& arquivo, ValorBinary& registro, ArquivoIO& arquivoIO) {
    arquivoEstaAberto(arquivo);
    arquivo.clear();
    arquivo.seekp(arquivoIO.copy * sizeof(ValorBinary), ios::beg);
    arquivo.flush();
    arquivo.write(reinterpret_cast<char*>(&registro), sizeof(ValorBinary));
    if (arquivoEstaEstavel(arquivo)) {
       arquivoIO.copy++;
       arquivo.flush();
       return true; 
    }
    return false;
}

bool escreverRegistroDesocupadoNoArquivo(fstream& arquivo, const ValorBinary& registro, ArquivoIO& arquivoIO) {
    arquivoEstaAberto(arquivo);
    ValorBinary registroDesocupado = registro;
    registroDesocupado.ocupado = false;
    arquivo.clear();
    arquivo.seekp(arquivoIO.write * sizeof(ValorBinary), ios::beg);
    arquivo.write(reinterpret_cast<char*>(&registroDesocupado), sizeof(ValorBinary));
    if (arquivoEstaEstavel(arquivo)) {
        arquivoIO.write++;
        arquivo.flush();
        return true;
    }
    return false;
}

bool arquivoEstaAberto(const fstream& arquivo) {
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        exit(-1);
    }
    return true;
}

priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> lerRegistrosIniciais(vector<fstream>& arquivos, const int& indiceArquivoVazio, vector<ValorBinary>& ultimoRegistroLidoDeCadaArquivo, vector<ArquivoIO> arquivosIO) {
    priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> heap;
    limparBufferStreamVector(arquivos);
    ValorBinary registro;
    unsigned long contadorFinalArquivo = quantosArquivosChegaramEOF(arquivosIO);

    if (contadorFinalArquivo == arquivos.size() - 1)
        return heap;

    for (unsigned long i = 0; i < arquivos.size() - 1; i++)
    {
        const int j = i;
        if (j != indiceArquivoVazio)
        {
            registro = lerRegistroDoArquivo(arquivos[i], arquivosIO[i]);
            if (registro.is_valido())
            {
                heap.push(registro);
                ultimoRegistroLidoDeCadaArquivo[registro.arquivoOrigem] = registro;
            }
        }
    }
    exibirPilhaValorBinary(heap);
    // exit(-1);
    return heap;
    
    // cout << "Acabou o loop inicial\n";

    // while (lerRegistro) {
    //     cout << "Entrou no loop principal\n";
    //     cout << "Ler Registro: " << lerRegistro<< endl;

    //     // Verificar se já acabou a memória ou os registros a serem lidos
    //     if (heapAux.size() >= static_cast<unsigned long>(memoria)){
            
    //         lerRegistro = false;
    //         unsigned long size = heapAux.size();
    //         for (unsigned long i = 0; i < size; i++)
    //         {
    //             registroAux = heapAux.top();
    //             heapAux.pop();

    //             // cout << "Termina?\n";
    //             heapValorBinary.push(registroAux);
    //             ultimoRegistroLidoDeCadaArquivo[registroAux.arquivoOrigem] = registroAux;

    //             // heapValorBinary.push(heapAux.top());
    //             // heapAux.pop();
    //         }
            
    //     } else {
    //         unsigned long contadorChoqueSequencias = quantosArquivosChocaramSequencias(arquivosIO);
    //         if (contadorChoqueSequencias == arquivos.size() - 1) {
    //             lerRegistro = false;
    //             cout << "CHOQUE DE MEMÓRIA\n\n";
    //             olharPonteirosMemoriaIO(arquivos, arquivosIO);// TESTE
    //             exit(-1);
    //         } else {
    //             contadorFinalArquivo = quantosArquivosChegaramEOF(arquivosIO);
    //             if (contadorFinalArquivo == arquivos.size() - 1) {
    //                 lerRegistro = false;
    //                 cout << "CHEGOU NO EOF\n\n";
    //             }
    //         }
    //     }

    //     registroAux = lerRegistroDoArquivo(arquivos[registroAux.arquivoOrigem], arquivosIO[registroAux.arquivoOrigem]); // TESTE_MODIFICAR_AGORA
    //     // olharPonteirosMemoriaIO(arquivos, arquivosIO); // TESTE_AQUI
    //     if (registroAux.is_valido()) {
    //         heapAux.push(registro);
    //     }
    //     loops++;
    // }
    // return heapValorBinary;
}

int esvaziarFilaDePrioridade(priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary>& heap, fstream& arquivo, const ValorBinary& registro, vector<ArquivoIO>& arquivosIO) {
    ValorBinary regInsert;
    int escritasEmArquivo = 0, i = 1;
    while (heap.size()) {
        regInsert = heap.top();
        heap.pop();
        regInsert.posicao = registro.posicao + i;
        regInsert.sequencia = registro.sequencia;
        regInsert.tamSequencia = registro.tamSequencia;
        regInsert.arquivoOrigem = registro.arquivoOrigem;
        escritasEmArquivo += escreverRegistroNoArquivo(arquivo, regInsert, arquivosIO[registro.arquivoOrigem]);
        i++;
    }
    return escritasEmArquivo;
}

int quantosArquivosChegaramEOF(const vector<ArquivoIO>& arquivoIO) {
    int contadorFinalArquivo = 0;
    for (const ArquivoIO& io: arquivoIO) {
        if (io.eof)
            contadorFinalArquivo++;
    }
    return contadorFinalArquivo;
}

ArquivoIO qualArquivoChegouEmEOF(const vector<ArquivoIO>& arquivoIO) {
    for (const ArquivoIO& io: arquivoIO) {
        if (io.eof)
            return io;
    }
    return ArquivoIO::criarInvalido();
}

int quantosArquivosChocaramSequencias(const vector<ArquivoIO>& arquivoIO) {
    int contadorChoqueSequencias = 0;
    for (const ArquivoIO& io: arquivoIO) {
        if (io.choqueSequencias)
            contadorChoqueSequencias++;
    }
    return contadorChoqueSequencias;
}

int somarTamanhoSequencias(const vector<ValorBinary>& registros) {
    int tamanhoSequencias = 0;

    for (unsigned long i = 0; i < registros.size(); i++) {
        if (registros[i].is_valido() && registros[i].ocupado == true)
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

bool copiarSequenciasTodosOsArquivos(vector<fstream>& arquivos, vector<ArquivoIO>& arquivosIO) {
    for (unsigned long i = 0; i < arquivos.size(); i++)
    {
        if (!copiarSequenciasRestantesProInicio(arquivos[i], arquivosIO[0])) {
            return false;
        }
    }
    return true;
}

bool copiarSequenciasRestantesProInicio(fstream& arquivo, ArquivoIO& arquivoIO) {
    bool vazio = arquivoVazio(arquivo);

    if (!vazio) {
        arquivo.seekg(arquivoIO.read * sizeof(ValorBinary), ios::beg);
        streampos posicaoListasALer = arquivo.tellg();
        arquivo.seekg(0, ios::end);
        streampos tamanhoArquivo = arquivo.tellg();
        // int quantidadeRegistrosTotais = tamanhoArquivo / sizeof(ValorBinary);
        int sizeOfRegistrosALer = tamanhoArquivo - posicaoListasALer;
        int registrosALer = sizeOfRegistrosALer / sizeof(ValorBinary);

        arquivoIO.read = registrosALer;
        arquivoIO.write = 0;
        ValorBinary registro;
        int sequenciaAtual, sequenciaInicial = 0;
        for (int i = 0; i < registrosALer; i++) {
            registro = lerRegistroDoArquivo(arquivo, arquivoIO);
            if (registro.is_valido()) {

                if (i == 0) {
                    sequenciaInicial = registro.sequencia;
                    sequenciaAtual = sequenciaInicial;
                }
                
                if (sequenciaAtual != registro.sequencia) {
                    sequenciaAtual++;
                }

                registro.sequencia = sequenciaAtual - sequenciaInicial;
                registro.ocupado =  true;
                escreverRegistroNoArquivoCopy(arquivo, registro, arquivoIO);
                
            }       
        }
        
        arquivo.close();
        truncarArquivoCopy(arquivoIO, registrosALer);
        arquivo = abrirArquivoDB(pegarPastaPages(), arquivoIO.origem);

        return true;
    }

    return false;
}

bool truncarArquivoCopy(ArquivoIO arquivoIO, int registrosCopiados) {
     // Caminho do arquivo agora na pasta "pages"
    string path = "pages/"+ to_string(arquivoIO.origem) + ".db";
    int f = open(path.c_str(), O_RDWR);
    if (f == -1) {
        perror("Erro ao abrir o arquivo");
        return false;
    }

    // Trunca o arquivo para deixar apenas os primeiros 10 registros
    if (ftruncate(f, registrosCopiados * sizeof(ValorBinary)) == -1) {
        perror("Erro ao truncar o arquivo");
        close(f);
        return false;
    }

    close(f);
    return true;
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

// TESTE_FUNCTION
void teste_function(vector<fstream>& arquivos) {
    ValorBinary registro;

    // DIFERENTE DO QUE O GPT FALOU PONTEIROS DE LEITURA E ESCRITA EM UM ARQUIVO NÃO SÃO INDEPENDETES
    // PARA ONDE UM VAI O OUTRO VAI
    // PONTEIROS DE ARQUIVOS DIFERENTES SÃO INDEPENDETES ENTRE SI, MAS NÃO EM SI MESMO

    // arquivos[0].seekp(0 * sizeof(registro), ios::beg);
    // arquivos[1].seekp(6 * sizeof(registro), ios::beg);
    // streampos posicao = arquivos[1].tellp(); // TESTE
    // cout << "Posição do ponteiro tellp[1]: " << posicao << endl << endl; // TESTE
    // arquivos[0].seekg(-2 * sizeof(registro), ios::end);
    // arquivos[0].read(reinterpret_cast<char*>(&registro), sizeof(ValorBinary));
    // exibirValorBinary(registro);
    // posicao = arquivos[0].tellp(); // TESTE
    // cout << "Posição do ponteiro tellp: " << posicao << endl << endl; // TESTE

    // arquivos[0].seekg(0, ios::beg);
    // posicao = arquivos[0].tellp(); // TESTE
    // cout << "Posição do ponteiro tellp: " << posicao << endl << endl; // TESTE

    // salvarArquivoNaMemoria(arquivos[0]);

    // ArquivoIO io = ArquivoIO();
    // for (int i = 0; i < 7; i++)
    // {
    //     io.exibir();
    //     lerRegistroDoArquivo(arquivos[0], io);
    // }
    // io.exibir();
    // vector<ValorBinary> ultimoRegistroLidoDeCadaArquivo(arquivos.size(), ValorBinary::criarInvalido());
    vector<ArquivoIO> arquivosIO(arquivos.size(), ArquivoIO());
    // arquivosIO[0].read = 6;
    // arquivosIO[1].read = 4;
    // arquivosIO[2].read = 4;

    // priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary>
    //     heap = lerRegistrosIniciais(arquivos, memoria, 3, ultimoRegistroLidoDeCadaArquivo, arquivosIO);

    // while (heap.size()) {
    //     cout << "Valor: " << heap.top().valor << " " << "Size heap: " << heap.size() << endl;
    //     heap.pop();
    // }

    // TESTE_FUNCTION -> copiarSequenciasRestantesProInicio
    arquivos[0].seekg(0, ios::end);
    streampos posicao = arquivos[0].tellg();
    int size = posicao / sizeof(ValorBinary);
    cout << "Size arquivos[0]: " << size << endl << endl;
    for (int i = 0; i < 3; i++)
    {
        registro = lerRegistroDoArquivo(arquivos[0], arquivosIO[0]);
        registro.exibir();
    }
    copiarSequenciasRestantesProInicio(arquivos[0], arquivosIO[0]);
    arquivos[0].seekg(0, ios::end);
    posicao = arquivos[0].tellg();
    size = posicao / sizeof(ValorBinary);
    cout << "Size arquivos[0]: " << size << endl;
    arquivosIO[0].read = 0;
    arquivosIO[0].write = 0;
    for (int i = 0; i < 3; i++)
    {
        registro = lerRegistroDoArquivo(arquivos[0], arquivosIO[0]);
        registro.exibir();
    }


    exit(-1);
}
