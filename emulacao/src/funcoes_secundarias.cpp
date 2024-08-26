// funcoes_secundarias.cpp
#include "funcoes_secundarias.h"
#include "estruturas.h"
#include "funcoes_gerais.h"
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void minhaFuncaoSecundaria() {
    cout << "Minha função secundaria foi chamada!" << endl;
}

// Funções Secundárias auxiliares
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

void exibirListaDeMatrizValorBinary(vector<vector<vector<ValorBinary>>>& listaDeMatriz) {
    for (unsigned long i = 0; i < listaDeMatriz.size(); i++) {
        cout << "Matriz Exibir\n";
        for (unsigned long j = 0; j < listaDeMatriz[i].size(); j++) {
            if (listaDeMatriz[i][j].size() != 0){
                cout << "Página " << i << endl << "Sequencia: " << j << endl;
                for (unsigned long k = 0; k < listaDeMatriz[i][j].size(); k++) {
                    cout << listaDeMatriz[i][j][k].valor << " ";
                }
                cout << endl << endl;
            }
        }
    }
}

void exibirValorBinary(const ValorBinary& registro) {
    cout << "Ocupado: " << registro.ocupado << endl;
    cout << "Registro Falso: " << registro.registroFalso << endl;
    cout << "Valor: " << registro.valor << endl;
    cout << "Sequencia: " << registro.sequencia << endl;
    cout << "Tamaho Sequencia: " << registro.tamSequencia << endl;
    cout << "Posicao: " << registro.posicao << endl;
    cout << "Arquivo de Origem: " << registro.arquivoOrigem << endl << endl;
}


void exibirPonteirosArquivos(vector<fstream>& arquivos) {
    for (fstream& arquivo: arquivos) {
        exibirPonteiroArquivo(arquivo);
    }

}

void exibirPonteiroArquivo(fstream& arquivo) {
    arquivoEstaEstavel(arquivo);
    streampos posicao = arquivo.tellg();
    arquivo.seekg(0, ios::end);
    streampos posicaoFim = arquivo.tellg();
    int totalReg = posicaoFim / sizeof(ValorBinary);
    int regPonteiro = posicao / sizeof(ValorBinary);

    cout << "Posição atual: " << posicao << endl;
    cout << "Registros atuais: " << regPonteiro << endl;
    cout << "Posição fim: " << posicaoFim << endl;
    cout << "Registradores fim: " << totalReg << endl << endl;

    arquivo.seekg(posicao * sizeof(ValorBinary), ios::beg);
}

// Imprimindo resultados
void exibirResultadoValorBinary(const vector<vector<vector<ValorBinary>>>& listaDeMatriz) {
    // cout << "fase " << fase << " " << beta << endl;
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

void exibirArquivosIO(vector<ArquivoIO> arquivosIO) {
    for (ArquivoIO& io : arquivosIO) {
        io.exibir();
    }
}

void olharArquivos(vector<fstream>& arquivos) {
    vector<vector<vector<ValorBinary>>> arquivosEmMemoria = salvarEstadoDosArquivosNaMemoria(arquivos);
    exibirResultadoValorBinary(arquivosEmMemoria);
    cout << endl << "////////////////////////////////////" << endl;
}

void olharPonteirosMemoria(vector<fstream>& arquivos) {
    exibirPonteirosArquivos(arquivos);
    olharArquivos(arquivos);
    // exit(-1);
}

void olharPonteirosMemoriaIO(vector<fstream>& arquivos, vector<ArquivoIO>& arquivosIO) {
    arquivosEstaveis(arquivos);
    exibirPonteirosArquivos(arquivos);
    exibirArquivosIO(arquivosIO);
    olharArquivos(arquivos);
    // exit(-1);
}

bool arquivosEstaveis(vector<fstream>& arquivos) {
    bool response = false;
    for (fstream& arq : arquivos) {
        response = arquivoEstaEstavel(arq);
    }
    return response;
}

void exibirPilhaValorBinary(priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> heapValorBinary) {
    int cont = 0;
    while (heapValorBinary.size())
    {
        ValorBinary valorBinary = heapValorBinary.top();
        cout << "PILHA " << cont << "º:   \n";
        valorBinary.exibir();
        cout << "Pilha\n";
        cout << endl << endl;
        heapValorBinary.pop();
        cont++;
    }
    
}