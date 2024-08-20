// funcoes_ordenacao_polifasica.cpp
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

#include "funcoes_ordenacao_polifasica.h"
#include "funcoes_secundarias.h"
#include "funcoes_gerais.h"

using namespace std;
namespace fs = std::filesystem;

void minhaFuncaoPolifasica() {
    cout << "Minha função Polifasica foi chamada!" << endl;
}

vector<int> calcularDivisaoSequenciasIniciais(const int& qtdSequenciasIniciais, const int& arqAbertos) {
    vector<vector<int>> sequencias;
    vector<int> sequencia(arqAbertos, 0), qtdSequenciasPorLinha(1, 0);
    sequencia[0] = 1;
    sequencias.push_back(sequencia);
    int maiorValorLinha = 0, posMaiorValorLinha = 0, posVazia = 0, qtdSequencias = 0;

    while (qtdSequenciasIniciais > qtdSequenciasPorLinha.back()) {
        // encontrar pos vazia e do maior elemento
        for (int i = 0; i < arqAbertos; i++) {
            if (sequencia[i] == 0) {
                posVazia = i;
            } else if (sequencia[i] > maiorValorLinha) { // em caso de serem iguais, ficamos com o primeiro maior valor
                maiorValorLinha = sequencia[i];
                posMaiorValorLinha = i;
            }            
        }

        // criar próxima linha do array
        for (int i = 0; i < arqAbertos; i++) {
            if (i == posVazia) {
                sequencia[i] = maiorValorLinha;
            } else if (i == posMaiorValorLinha) {
                sequencia[i] = 0;
            } else {
                sequencia[i] += maiorValorLinha;
            }
        }

        qtdSequencias = somaValoresLista(sequencia);
        
        qtdSequenciasPorLinha.push_back(qtdSequencias);
        sequencias.push_back(sequencia);
        maiorValorLinha = 0; 
        posMaiorValorLinha = 0; 
        posVazia = 0;

    }
    qtdSequenciasPorLinha.erase(qtdSequenciasPorLinha.begin());
    // exibirListaDeListasInt(sequencias);

    return sequencias.back();
}

int somaValoresLista(vector<int>& lista) {
    int somatorio = 0;
    for (size_t i = 0; i < lista.size(); i++) {
        somatorio += lista[i];
    }
    return somatorio;
}

void salvarListasEmArquivos(const vector<vector<int>>& listas, const int& arquivosAbertos, const int& maxSequenciasIniciais) {
    fs::path folder = criarPasta("pages");
    vector<fstream> arquivos = criarArquivosDB(folder, arquivosAbertos);
    vector<int> divisaoSequencias = calcularDivisaoSequenciasIniciais(maxSequenciasIniciais, arquivosAbertos);
    divisaoSequencias.erase(remove(divisaoSequencias.begin(), divisaoSequencias.end(), 0), divisaoSequencias.end());
    

    // IMPLEMENTAR ESTRUTURA DE DADOS QUE SERÁ ARMAZENADA
    // 


    fecharArquivosDB(arquivos);
}