#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Estrutura para armazenar o valor e sua origem
struct Valor {
    int valor;
    int origem;
    int linha;
};

// Comparador para a fila de prioridade
struct Comparador {
    bool operator()(const Valor& a, const Valor& b) {
        return a.valor > b.valor; // Para min-heap
    }
};

void mergeArquivos(int paginasMerge) {
    fs::path folder = "pages";
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }

    int linhaAtual = 1;
    vector<bool> finalLinha, finalArquivo;
    vector<ifstream> arquivos(paginasMerge);
    priority_queue<Valor, vector<Valor>, Comparador> minHeap;

    for (int i = 0; i < paginasMerge; i++) {
        finalLinha.push_back(false);
        finalArquivo.push_back(false);
    }
    

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
        if (finalLinha[indiceArquivo] || finalArquivo[indiceArquivo])
            return leuAlgo;

        if (arquivos[indiceArquivo].peek() == '\n') {
            finalLinha[indiceArquivo] = true;
            return leuAlgo;
        }
        if (arquivos[indiceArquivo].peek() == EOF) {
            finalArquivo[indiceArquivo] = true;
            return leuAlgo;
        }
        if (arquivos[indiceArquivo] >> valor) {
            minHeap.push({valor, indiceArquivo, linha});
            leuAlgo = true;
        } else {
            // arquivos[indiceArquivo].clear();
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

        // Processar todos os valores da linha atual
        while (!minHeap.empty()) {
            Valor menor = minHeap.top();
            minHeap.pop();
            int resto = linhaAtual % paginasMerge;
            fs::path nomeArquivoSaida = folder / (to_string(resto + paginasMerge) + ".txt");
            cout << resto << " = " << linhaAtual << " % " << paginasMerge << endl;
            cout << "pages/" << to_string(resto + paginasMerge) << ".txt" << endl << endl;
            ofstream arquivoSaida(nomeArquivoSaida, ios_base::app);
            if (!arquivoSaida.is_open()) {
                cerr << "Erro ao abrir o arquivo de saída " << nomeArquivoSaida << endl;
                return;
            }
            arquivoSaida << menor.valor << " ";
            lerValorLinha(menor.origem, menor.linha);

            if (minHeap.empty()) {
                arquivoSaida << endl;
                arquivoSaida.close();
            }
        }

        // Verificar se há mais linhas a serem lidas
        bool finalLinhaBool = false;
        linhaAtual++;
        for (int i = 0; i < paginasMerge; ++i) {
            if (!arquivos[i].eof()) { // !finalArquivo[i]
                leituraAtiva = true;
                if (finalLinha[i]){
                    finalLinha[i] = false;
                    int valor = 0;
                    if (arquivos[i] >> valor) {
                        minHeap.push({valor, i, linhaAtual});
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

int main() {
    int paginasMerge = 2;
    // cout << "Digite o número de arquivos para mesclar: ";
    // cin >> paginasMerge;

    mergeArquivos(paginasMerge);

    return 0;
}
