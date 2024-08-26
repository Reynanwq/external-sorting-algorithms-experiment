// funcoes_ordenacao_polifasica.cpp
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <limits>
#include <queue>
#include <cstdlib>

#include "funcoes_ordenacao_polifasica.h"
#include "funcoes_secundarias.h"
#include "funcoes_gerais.h"
#include "estruturas.h"

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

int salvarListasEmArquivosBinary(const vector<vector<int>>& listas, const int& arquivosAbertos, const int& maxSequenciasIniciais) {
    fs::path folder = criarPasta("pages");
    vector<fstream> arquivos = criarArquivosDB(folder, arquivosAbertos);
    
    vector<int> divisaoSequencias = calcularDivisaoSequenciasIniciais(maxSequenciasIniciais, arquivosAbertos);
    vector<int> divisaoSequenciasCopia;
    ValorBinary binary;
    int escritasEmArquivo = 0;

    // Retira o '0' da lista para que as 'r' sequências iniciais sejam armazenadas nos primeiros 'k-1' arquivos
    divisaoSequencias.erase(remove(divisaoSequencias.begin(), divisaoSequencias.end(), 0), divisaoSequencias.end());

    // Guarda as sequências nos arquivos que ainda podem receber sequências
    int arquivosEscrita = arquivosAbertos - 1;
    // contadorSequencia no final será semelhante à 'divisaoSequencias - 1', já que ele representa o indice da sequencia que está sendo armazenada
    vector<int> contadorSequencia(arquivosEscrita, -1);

    for (unsigned long i = 0; i < listas.size(); i++) {
        // Encontrar qual o indice do arquivo para armazenar a sequencia 'i'
        int resto = i % arquivosEscrita;
        bool achouResto = false;
        while (!achouResto) {
            if (resto == arquivosEscrita)
                resto = 0;
            
            if (divisaoSequencias[resto] > 0) {
                divisaoSequencias[resto]--;
                contadorSequencia[resto]++;
                achouResto = true;
            } else
                resto++;
        }
        
        // Armazena valores
        for (unsigned long j = 0; j < listas[i].size(); j++) {
            binary.ocupado = true;
            binary.registroFalso = false;
            binary.valor = listas[i][j];
            binary.sequencia = contadorSequencia[resto];
            binary.tamSequencia = listas[i].size();
            binary.posicao = j;
            binary.arquivoOrigem = resto;
            arquivos[resto].write(reinterpret_cast<char*>(&binary), sizeof(ValorBinary));
            escritasEmArquivo++;
        }
    }

    // Adiciona sequências unitárias com flag de falso e com o maior valor possível
    divisaoSequenciasCopia = divisaoSequencias;
    binary.ocupado = true;
    binary.registroFalso = true;
    binary.valor = numeric_limits<int>::max();
    binary.tamSequencia = 1;
    binary.posicao = 0;
    for (unsigned long i = 0; i < divisaoSequenciasCopia.size(); i++) {
        for (int j = 0; j < divisaoSequenciasCopia[i]; j++) {
            divisaoSequencias[i]--;
            contadorSequencia[i]++;
            binary.sequencia = contadorSequencia[i];
            binary.arquivoOrigem = i;
            arquivos[i].write(reinterpret_cast<char*>(&binary), sizeof(ValorBinary));
            escritasEmArquivo++;
        }   
    }
    
    fecharArquivosDB(arquivos);
    return escritasEmArquivo;
}

pair<int, double> mergePolifasico(vector<fstream>& arquivos, const int& memoria) {
    int fase = 0, quantidadeRegistros = 0, quantidadeSequencias = 0, escritasEmArquivo = 0;
    double beta = 0, primeiroBeta = 0;
    pair<int, double> retornoEscritasEBeta;
    vector<vector<vector<ValorBinary>>> arquivosEmMemoria;
    int teste = 0;
    while (quantosArquivosCheios(arquivos) > 1) {
        arquivosEmMemoria = salvarEstadoDosArquivosNaMemoria(arquivos);
        if (fase == 0) {
            primeiroBeta = beta;
            quantidadeRegistros = quantidadeDeRegistrosValorBinary(arquivosEmMemoria);
        }
        quantidadeSequencias = quantidadeDeSequenciasValorBinary(arquivosEmMemoria);
        beta = calcularBeta(memoria, quantidadeSequencias, quantidadeRegistros);
        // cout << "memoria: " << memoria << "\nQuantidade de Sequencias: " << quantidadeSequencias << "\nQuantidade de Registros: " << quantidadeRegistros << endl;
        imprimirResultadoValorBinary(arquivosEmMemoria, fase, beta);
        
        // mergeRodadaPolifasica(arquivosEmMemoria);

        priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> heap;
        // inserirValoresNaHeap(arquivosEmMemoria, )

        mergeRodadaPolifasica(arquivos, arquivosEmMemoria);

        // exibirPonteirosArquivos(arquivos);
        // olharArquivos(arquivos);
        
        // teste_function(arquivos); // TESTE_FUNCTION

        if (teste == 0) {
            // escritasEmArquivo += mergePolifasicoRodada(arquivos, memoria); // TESTE_MODIFICAR_AGORA
            escritasEmArquivo += mergeRodadaPolifasica(arquivos, arquivosEmMemoria);
            teste++;
        } else {
            break;
        }

        fase++;
    }
    retornoEscritasEBeta = make_pair(escritasEmArquivo, primeiroBeta);
    return retornoEscritasEBeta;
}

int mergePolifasicoRodada(vector<fstream>& arquivos, const int& memoria) {
    if (memoria) {};
    bool continuaMerge = true;
    int indiceArquivoVazio = 0, escritasEmArquivo = 0, sequencia = 0, posicao = 0, ultimoArquivoLido = 0;
    unsigned long contadorChoqueSequencias; // contadorFinalArquivo;
    ValorBinary registro, registroBackup;
    const ValorBinary registroInvalido = ValorBinary::criarInvalido();
    // vector<int> arquivosChoqueSequenciaOuEOF(arquivos.size(), 0);

    vector<ArquivoIO> arquivosIO = ArquivoIO::criarSetOrigem(arquivos.size());
    vector<ValorBinary> ultimoRegistroLidoDeCadaSeqArquivo(arquivos.size(), registroInvalido);
    priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> heapValorBinary;

    indiceArquivoVazio = qualOPrimeiroArquivoVazio(arquivos);


    heapValorBinary = lerRegistrosIniciais(arquivos, indiceArquivoVazio, ultimoRegistroLidoDeCadaSeqArquivo, arquivosIO);
    int tamSequencia = somarTamanhoSequencias(ultimoRegistroLidoDeCadaSeqArquivo);

    // Implementar limpeza dos registros que estão na frente
    // E fazer o trunc com 'c' para diminuir o arquivo

    while (continuaMerge) {
        exibirPilhaValorBinary(heapValorBinary);
        registro = heapValorBinary.top();
        registroBackup = registro;
        heapValorBinary.pop();
        registro.posicao = posicao;
        registro.sequencia = sequencia;
        registro.tamSequencia = tamSequencia;
        registro.arquivoOrigem = indiceArquivoVazio;
        escritasEmArquivo += escreverRegistroNoArquivo(arquivos[indiceArquivoVazio], registro, arquivosIO[indiceArquivoVazio]);
        
        olharArquivos(arquivos); // TESTE_AQUI
        // Salva indice do ultimo arquivo para poder ler o proximo valor desse arquivo
        ultimoArquivoLido = registroBackup.arquivoOrigem;

        // Guardando último registro escrito no arquivo que era vazio para poder esvaziar a pilha posteriormente
        registroBackup = registro;

        // Lê novo registro e verifica se ele é valido
        // Se não for, verifica se é necessário começar a ler de uma nova sequência ou se todos os valores da sequênia atual foram lidos
        registro = lerRegistroDoArquivo(arquivos[ultimoArquivoLido], arquivosIO[ultimoArquivoLido]); // TESTE_MODIFICAR_AGORA
        if (registro.is_valido()) {
            heapValorBinary.push(registro);
            ultimoRegistroLidoDeCadaSeqArquivo[ultimoArquivoLido] = registro;
        } else {

            contadorChoqueSequencias = quantosArquivosChocaramSequencias(arquivosIO);
            
            // Verifica se a todos os arquivos que estão mergeando já não tem mais nada para ler da sequencia atual
            if (contadorChoqueSequencias == arquivos.size() - 1) {
                // esvaziarPilha da sequencia atual e prepara para a próxima
                escritasEmArquivo += esvaziarFilaDePrioridade(heapValorBinary, arquivos[indiceArquivoVazio], registroBackup, arquivosIO);
                posicao = -1;
                sequencia++;

                ArquivoIO arquivoIO = qualArquivoChegouEmEOF(arquivosIO); // TESTE_CORRIGIR

                if (arquivoIO.is_valido())
                    continuaMerge = false;
                else {

                    for (unsigned long i = 0; i < ultimoRegistroLidoDeCadaSeqArquivo.size(); i++) {
                        ultimoRegistroLidoDeCadaSeqArquivo[i] = registroInvalido;
                    }
                    tamSequencia = somarTamanhoSequencias(ultimoRegistroLidoDeCadaSeqArquivo);
                    
                    heapValorBinary = lerRegistrosIniciais(arquivos, indiceArquivoVazio, ultimoRegistroLidoDeCadaSeqArquivo, arquivosIO);
                }

                for (unsigned long i = 0; i < arquivosIO.size(); i++)
                {
                    arquivosIO[i].choqueSequencias = false;
                }
            }
        }
        posicao++;
    }
    copiarSequenciasTodosOsArquivos(arquivos, arquivosIO);


    return escritasEmArquivo;
}

int mergeRodadaPolifasica(vector<fstream>& arquivos, vector<vector<vector<ValorBinary>>>& arquivosEmMemoria) {
    int escritasArquivo = 0;
    int indiceVazio = qualSequenciaVazia(arquivosEmMemoria);
    cout << "Indice página: " << indiceVazio << endl;
    if (arquivos.size()){};
    priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> heap;
    inserirValoresNaHeap(arquivosEmMemoria, heap,indiceVazio);

    return escritasArquivo;
}

// Outras
int qualSequenciaVazia(vector<vector<vector<ValorBinary>>>& arquivosEmMemoria) {
    for (unsigned long i = 0; i < arquivosEmMemoria.size(); i++) {
        for (unsigned long j = 0; j < arquivosEmMemoria[i].size(); j++) {
            if (arquivosEmMemoria[i][j].empty()) {
                return i; // Retorna o índice do primeiro vetor externo onde há uma lista vazia.
            }
        }
    }
    return -1; // Retorna -1 se nenhuma lista vazia for encontrada.
}

int qualMaiorSequencia(vector<vector<vector<ValorBinary>>>& arquivosEmMemoria) {
    unsigned long maior = 0;
    for (unsigned long i = 0; i < arquivosEmMemoria.size(); i++) {
        if (arquivosEmMemoria[i].size() > maior) {
            maior = arquivosEmMemoria[i].size();
        }
    }
    return maior;
}

vector<vector<vector<ValorBinary>>> inserirValoresNaHeap(
    vector<vector<vector<ValorBinary>>>& arquivosEmMemoria,
    priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary>& heap,
    const int& indiceVazio
) {
    // Passar todos os valores das outras matrizes para a heap mínima
    // int qtdListas = qualMaiorSequencia(arquivosEmMemoria);
    if (heap.size())
    {
        /* code */
    }
    
    int menorQtdSequencias = qualAMenorQUantidadeDeSequencias(arquivosEmMemoria);
    vector<vector<ValorBinary>> novoArquivo(menorQtdSequencias); // Sequencias para serem colocadas no arquivo vazio
    vector<ValorBinary> temp;
    ValorBinary valorAnterior;
    vector<vector<vector<ValorBinary>>> copiarArquivos(arquivosEmMemoria.size(), novoArquivo);

    // exibirListaDeMatrizValorBinary(arquivosEmMemoria);

    for (unsigned long i = 0; i < arquivosEmMemoria.size(); i++) {
        if (i == static_cast<unsigned long>(indiceVazio)) continue; // Ignorar a matriz vazia

        cout << "\noi\n";
        int j = 0;
        for (auto& lista : arquivosEmMemoria[i]) {
            int k = 0;
            for (auto& valor : lista) {
                if (valor.sequencia < menorQtdSequencias)
                {
                    novoArquivo[valor.sequencia].push_back(valor);
                    cout << valor.valor << endl ;
                } else { 
                    valorAnterior = valor;
                    valor.sequencia = -1;
                }
                k++;
            }
            // lista.clear();
            j++;
        }
    }

    cout << "Novo arquivo: " << novoArquivo[0][0].valor << endl ;
    cout << "Imprime\n";
    // exibirListaDeMatrizValorBinary(arquivosEmMemoria);

    return arquivosEmMemoria;
}

int qualAMenorQUantidadeDeSequencias(vector<vector<vector<ValorBinary>>>& arquivosNaMemoria) {
    vector<int> sequencias(arquivosNaMemoria.size());
    for (unsigned long i = 0; i < arquivosNaMemoria.size(); i++)
    {
        sequencias[i] = arquivosNaMemoria[1].size();
    }
    auto menorValor = min_element(sequencias.begin(), sequencias.end());
    if (menorValor != sequencias.end())
    {
        return *menorValor;
    } else {
        return -1;
    }
}

