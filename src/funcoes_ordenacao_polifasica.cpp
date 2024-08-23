// funcoes_ordenacao_polifasica.cpp
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <limits>

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
        cout << "memoria: " << memoria << "\nQuantidade de Sequencias: " << quantidadeSequencias << "\nQuantidade de Registros: " << quantidadeRegistros << endl;
        imprimirResultadoValorBinary(arquivosEmMemoria, fase, beta);
        if (teste == 0) {
            escritasEmArquivo += mergePolifasicoRodada(arquivos, memoria);
            teste++;
        } else {
            break;
        }

        fase++;
    }

    // Próximos passos
    // chamar merge aqui, retornar o Beta 0
    // Criar função para salvar estado atual das páginas em uma estrutura de ValorBinary
    // Criar função para imprimir estrutura, ela recebe a fase atual, Beta e a estrutura com ValorBinary
    // Depois que as firulas passaram chama o merge real
    // Criar / chamar merge real 
        // Verifica qual é a página vazia
        // criar vector para ser utilizado ao final, nele se guarda o tamanho das sequencias de cada registro e põe para -1 a página vazia
        // Lê um valor de cada página não vazia e seta os seus endereços como não ocupado
        // Loop começa adicionando o menor valor ao arquivo vazio
        // Condição de parada - Criar função que calcula quantas páginas vazias existem - se for igual a qtdArquivos -1 -> para
            // O tamanho de sequencia desse registro será a soma do tamanho das sequencias dos n registros iniciais
            // Lê um valor da pagina do registro que foi armazenado e 
            // Verifica se o valor lido é da próxima sequência - if
                // caso seja é necessário retornar o ponteiro de leitura daquele arquivo em 1 posicao
            // Verifia também se não chegou no .eof para poder colocar na heap - else if
                // Se não chegou coloca no heap mínima e seta seu endereço no arquivo para não ocupado
            // Verifica se um dos três arquivos chegou no fim  | .eof - if
                // Caso sim - Criar função para Copiar os valores do final dos arquivos que não chegaram no fim para frente
                // chamar função para truncar arquivo que chegou ao final

        // PS - E SE A QUANTIDADE DE SEQUENCIAS FOREM MENORES QUE OS ARQUIVOS QUE PODEM SER ABERTOS ?

    // RETORNAR O PAIR<PAIR> PARA RETORNAR A QUANTIDADE DE REGISTROS - INT DOUBLE INT
    retornoEscritasEBeta = make_pair(escritasEmArquivo, primeiroBeta);
    return retornoEscritasEBeta;
}

int mergePolifasicoRodada(vector<fstream>& arquivos, const int& memoria) {
    bool continuaMerge = true;
    ValorBinary registro, registroBackup;
    const ValorBinary registroInvalido = criarValorBinaryInvalido();
    vector<int> arquivosChoqueSequenciaOuEOF(arquivos.size(), 0);
    vector<ValorBinary> ultimoRegistroLidoDeCadaArquivo(arquivos.size(), criarValorBinaryInvalido());
    priority_queue<ValorBinary, vector<ValorBinary>, ComparadorValorBinary> heapValorBinary;
    int indiceArquivoVazio = 0, escritasEmArquivo = 0, sequencia = 0, posicao = 0, ultimoArquivoLido = 0;
    int arquivoEOF = 0;
    unsigned long totalChoquesOuEOF = 0;

    int tamSequencia = somarTamanhoSequencias(ultimoRegistroLidoDeCadaArquivo);
    indiceArquivoVazio = qualOPrimeiroArquivoVazio(arquivos);
    heapValorBinary = lerRegistrosIniciais(arquivos, memoria, indiceArquivoVazio, ultimoRegistroLidoDeCadaArquivo, sequencia);


    // vector<pair<int, int>> ponteirosIO(arquivos.size(), make_pair(0, 0)); // ISSO AQUI VAI TER QUE SER PASSADO POR REFERÊNCIA PELA FUNÇÃO QUE CHAMA ESTA FUNÇÃO
    // FUNÇÕES QUE PRECISAM MUDAR
    // escreverRegistroNoArquivo
    // lerRegistroDoArquivo
    // qualArquivoChegouEmEOF // DESCOBRIR NOVA FORMA DE DETERMINAR O ESTADO DE EOF DO ARQUIVO JÁ QUE ESTAMOS LIMPANDO TUDO
    // Implementar limpeza dos registros que estão na frente
    // E fazer o trunc com 'c' para diminuir o arquivo

    while (continuaMerge) {
        // Preparando registro com valores para ser armazenado em um novo arquivo com nova sequencia
        registro = heapValorBinary.top();
        registroBackup = registro;
        heapValorBinary.pop();
        registro.posicao = posicao;
        registro.sequencia = sequencia;
        registro.tamSequencia = tamSequencia;
        registro.arquivoOrigem = indiceArquivoVazio;
        escritasEmArquivo += escreverRegistroNoArquivo(arquivos[indiceArquivoVazio], registro);
        cout << "Teste\n";
        // Salva indice do ultimo arquivo para poder ler o proximo valor desse arquivo
        ultimoArquivoLido = registroBackup.arquivoOrigem;

        // Guardando último registro escrito no arquivo que era vazio para poder esvaziar a pilha posteriormente
        registroBackup = registro;

        // Lê novo registro e verifica se ele é valido
        // Se não for, verifica se é necessário começar a ler de uma nova sequência ou se todos os valores da sequênia atual foram lidos
        registro = lerRegistroDoArquivo(arquivos[ultimoArquivoLido], sequencia);
        if (registro.ocupado == true) {
            heapValorBinary.push(registro);
            ultimoRegistroLidoDeCadaArquivo[ultimoArquivoLido] = registro;
        } else {
            // Guarda o indice de qual arquivo já não tem mais registros da sequencia atual
            totalChoquesOuEOF = 0;
            if (registro == registroInvalido)
                arquivosChoqueSequenciaOuEOF[ultimoArquivoLido] = 1;
            
            // Conta se a todos os arquivos que estão mergeando já não tem mais nada para ler da sequencia atual
            for (unsigned long i = 0; i < arquivosChoqueSequenciaOuEOF.size(); i++) {
                if (arquivosChoqueSequenciaOuEOF[i] == 1)
                    totalChoquesOuEOF++;
            }

            // Verifica se a todos os arquivos que estão mergeando já não tem mais nada para ler da sequencia atual
            if (totalChoquesOuEOF == arquivos.size() - 1) {
                // esvaziarPilha da sequencia atual e prepara para a próxima
                escritasEmArquivo += esvaziarFilaDePrioridade(heapValorBinary, arquivos[indiceArquivoVazio], registro);
                posicao = -1;
                sequencia++;
                fill(arquivosChoqueSequenciaOuEOF.begin(), arquivosChoqueSequenciaOuEOF.end(), 0);
                
                // se todos os arquivos não tem mais nada para ser lido e e chegou no EOF do que tinha menor qtd seq então para o while
                arquivoEOF = qualArquivoChegouEmEOF(arquivos, indiceArquivoVazio);
                if (arquivoEOF > -1)
                    continuaMerge = false;
                else {
                    // Se não chama registros das próximas sequencias
                    // chamar função que coloca na heap os próximos m valores das proximas sequencias
                    heapValorBinary = lerRegistrosIniciais(arquivos, memoria, indiceArquivoVazio, ultimoRegistroLidoDeCadaArquivo, sequencia);
                }
            }
        }
        posicao++;
    }

    // Implementar limpeza dos registros que estão na frente
    // E fazer o trunc com 'c' para diminuir o arquivo

    return escritasEmArquivo;
}