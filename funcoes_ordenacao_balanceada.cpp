// funcoes_ordenacao_balanceada.cpp
#include "funcoes_ordenacao_balanceada.h"
#include "funcoes_gerais.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void minhaFuncaoBalanceada() {
    cout << "Minha função balanceada foi chamada!" << endl;
}

// Imprimir saída ordenação balanceada
void imprimirSaida(int fase, float beta, const vector<vector<vector<int>>>& sequencias){
    cout << "fase " << fase << " " << beta << endl;
    for (unsigned long i = 0; i < sequencias.size(); i++) {
        if (sequencias[i].size()) {
            cout << i + 1 << ": ";
            for (unsigned long int j = 0; j < sequencias[i].size(); j++){
                for (unsigned long int k = 0; k < sequencias[i][j].size(); k++) {
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
    for (unsigned long i = 0; i < sequencias.size(); i++) {
        if (sequencias[i].size()) {
            cout << i + 1 + incrementoPaginas << ": ";
            for (unsigned long j = 0; j < sequencias[i].size(); j++){
                for (unsigned long k = 0; k < sequencias[i][j].size(); k++) {
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

// Salvar dados
void salvarListasEmArquivos(const vector<vector<int>>& listas, int arquivosIntercalados) {
    fs::path folder = criarPasta("pages");

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

// Merge arquivos fases
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

// Intercalar arquivos
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


