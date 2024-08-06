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
            // std::cout << "Arquivo criado: " << filePath << endl;
        } else {
            // cerr << "Erro ao criar o arquivo: " << filePath << endl;
        }
    }
}

void mergeArquivos(int paginasMerge) {
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
            bool leuValor = lerValorLinha(menor.origem, menor.linha);
            // if (leuValor)
            // {
                // std::cout << "LEU\n";
            // } else {
                // std::cout << "Não LEU\n";
            // }
            
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
        bool finalLinhaBool = false;
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

int main() {
    int paginasMerge = 2;
    // std::cout << "Digite o número de arquivos para mesclar: ";
    // cin >> paginasMerge;

    mergeArquivos(paginasMerge);

    return 0;
}
