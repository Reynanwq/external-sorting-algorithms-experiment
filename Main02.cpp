#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>

using namespace std;

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

// Funções para os métodos de ordenação externa
void ordenacaoBalanceada(vector<int>& dados, int m, int k, int r);
void ordenacaoPolifasica(vector<int>& dados, int m, int k, int r);
void ordenacaoCascata(vector<int>& dados, int m, int k, int r);

// Funções auxiliares
// void gerarSequenciasIniciais(vector<int>& dados, int m, int r);
vector<vector<int>> gerarSequenciasIniciais(vector<int> dados, int m, int r);
void intercalaSequencias(vector<int>& dados, int m, int k);
void calcularMetricas(const vector<int>& dados, int r, int k);

// Funções para processamento e geração de dados
vector<int> gerarDadosAleatorios(int n);
void exibirResultados(const vector<int>& dados, int r, int k);

// Funções para criação e processamento da heap mínima
void desmarcarTodosElementos(priority_queue<Element, vector<Element>, Compare>& heap);

// Funções para exibir
void exibirListaDeListasInt(vector<vector<int>>& listas);
void exibirListaInt(vector<int>& lista, string nome);
void exibirHeap(priority_queue<Element, vector<Element>, Compare> heap);

int main() {
    // Exemplo de uso
    int m = 3;  // Capacidade da memória interna
    int k = 4;  // Número de arquivos abertos
    int r = 3;  // Número de sequências iniciais
    int n = 17; // Número de registros

    // vector<int> dados = gerarDadosAleatorios(n);
    vector<int> dados = {7, 1, 5, 6, 3, 8, 2, 10, 4, 9, 1, 3, 7, 4, 1, 2, 3};
    vector<vector<int>> sequencias = gerarSequenciasIniciais(dados, m, r);

    // Escolher o método de ordenação
    char metodo = 'B'; // Pode ser 'B', 'P' ou 'C'

    // switch (metodo) {
    //     case 'B':
    //         ordenacaoBalanceada(dados, m, k, r);
    //         break;
    //     case 'P':
    //         ordenacaoPolifasica(dados, m, k, r);
    //         break;
    //     case 'C':
    //         ordenacaoCascata(dados, m, k, r);
    //         break;
    //     default:
    //         cerr << "Método de ordenação inválido!" << endl;
    //         return 1;
    // }

    // intercalaSequencias(dados, m, k);
    // exibirResultados(dados, r, k);

    return 0;
}

// Função para ordenação balanceada multi-caminhos
void ordenacaoBalanceada(vector<int>& dados, int m, int k, int r) {
    gerarSequenciasIniciais(dados, m, r);
    // Implementar a ordenação balanceada multi-caminhos
    cout << "Ordenação Balanceada Multi-Caminhos ainda não implementada." << endl;
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


// Função para intercalar sequências
void intercalaSequencias(vector<int>& dados, int m, int k) {
    // Estrutura de dados para armazenar sequências iniciais
    vector<vector<int>> sequencias;
    gerarSequenciasIniciais(dados, m, sequencias.size());

    // Fila de prioridade para armazenar os elementos e suas origens
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Vetor de índices para manter o acompanhamento da posição de cada sequência
    vector<int> indices(sequencias.size(), 0);

    // Inicializar a fila de prioridade com o primeiro elemento de cada sequência
    for (int i = 0; i < sequencias.size(); ++i) {
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
        if (++indices[origem] < sequencias[origem].size()) {
            pq.push(make_pair(sequencias[origem][indices[origem]], origem));
        }
    }

    // Copiar os dados ordenados de volta para o vetor de dados original
    dados = dadosOrdenados;
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
            cout << "ADD HEAP" << endl;
            if (ultimoPush < dados.front()) {
                heap.push(Element(dados.front()));
            } else {
                heap.push(Element(dados.front(), true));
            }
            dados.erase(dados.begin());

        } else {
            cout << "ADD SEQUENCIA" << endl;
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
            exibirListaInt(dados, "Registros");
            exibirHeap(heap);
            exibirListaInt(sequenciaAtual, "Sequencia Atual");
            cout << "Heap mínima: " << heap.top().value << endl;
            cout << "Marcado: " << heap.top().marked << endl;
            exibirListaDeListasInt(sequencias);
    }

    // Garantir que geramos pelo menos m sequências
    while (sequencias.size() < m) {
        sequencias.push_back({});
    }

    // Exibir sequências geradas para depuração
    for (int i = 0; i < sequencias.size(); ++i) {
        cout << "Sequência " << i + 1 << ": ";
        for (int valor : sequencias[i]) {
            cout << valor << " ";
        }
        cout << endl;
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

// Função para calcular beta
double calcularBeta(const vector<vector<int>>& sequencias, int m, int j) {
    double soma = 0;
    for (const auto& seq : sequencias) {
        soma += seq.size();
    }
    return (1.0 / m) * soma / sequencias.size();
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