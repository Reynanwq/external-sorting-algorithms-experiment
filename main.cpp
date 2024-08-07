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
void ordenacaoPolifasica(vector<int>& dados, int m, int k, int r);
void ordenacaoCascata(vector<int>& dados, int m, int k, int r);


// Funções Secundárias auxiliares
    // Funções para exibir - Importar no arquivo balanceado - pode usar ou não
void exibirListaDeListasInt(vector<vector<int>>& listas);
void exibirListaInt(vector<int>& lista, string nome);
void exibirHeap(priority_queue<Element, vector<Element>, Compare> heap);

// Funções Balanceada
void ordenacaoBalanceada(vector<vector<int>>& sequencias, int memoria, int arquivosTotais, int r, int nRegistros, vector<pair<int, float>> listBetas, int& escritasArquivo);

float intercalaSalvarCalcularBalanceada(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas );
void intercalaSequenciasFaseUmBalanceada(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo);
void intercalaSequenciasFaseDoisBalanceada(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo);

void salvarListasEmArquivos(const vector<vector<int>>& listas, int arquivosIntercalados);

void mergeArquivosFaseUm(int& paginasMerge, int& escritasArquivo);
void mergeArquivosFaseDois(int paginasMerge, int& escritasArquivo);

void salvarEstadoDasPaginas(int fase, int arquivosIntercalados, vector<vector<vector<int>>>& dadosEstado);


int main() {
    minhaFuncaoGeral();
    minhaFuncaoSecundaria();
    minhaFuncaoBalanceada();
    minhaFuncaoPolifasica();
    minhaFuncaoCascata();

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

float intercalaSalvarCalcularBalanceada(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas ) {
    salvarEstadoDasPaginas(fase, arquivosIntercalados, estadoInicioFase);
    int nSequencias = calcularNumeroSequencias(estadoInicioFase);
    float beta = calcularBeta(memoria, nSequencias, nRegistros);
    listBetas.push_back(make_pair(fase, beta)); // Salvando estado de beta para calcular desempenho
    return beta;
}

void intercalaSequenciasFaseUmBalanceada(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo) {
    float beta = intercalaSalvarCalcularBalanceada(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas);
    imprimirSaida(fase, beta, estadoInicioFase);
    mergeArquivosFaseUm(arquivosIntercalados, escritasArquivo);
    criarLimparArquivosRange(0, arquivosIntercalados); // Apaga conteúdo antigo
}

void intercalaSequenciasFaseDoisBalanceada(int& fase, int& arquivosIntercalados, int& nRegistros, int& memoria, vector<vector<vector<int>>>& estadoInicioFase, vector<pair<int, float>> listBetas, int& escritasArquivo) {
    float beta = intercalaSalvarCalcularBalanceada(fase, arquivosIntercalados, nRegistros, memoria, estadoInicioFase, listBetas);
    imprimirSaidaIncremento(fase, beta, estadoInicioFase, arquivosIntercalados);
    mergeArquivosFaseDois(arquivosIntercalados, escritasArquivo);
    criarLimparArquivosRange(arquivosIntercalados, arquivosIntercalados * 2); // Apaga conteúdo antigo
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
    
    criarLimparArquivosRange(paginasMerge, paginasMerge * 2);

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
    criarLimparArquivosRange(oInicio, oFim);

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