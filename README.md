# external-sorting-algorithms-experiment
Universidade Federal da Bahia (UFBA)
Instituto de Computação (IME)
Departamento de Ciência da Computação (DCC)
MATA 54 - Estrutura de Dados e Algoritmos II – 2024.1 – Prof. George Lima
Estudo experimental de métodos de ordenação externa
1. Objetivo, contexto e informações iniciais

Este trabalho tem como objetivo consolidar os conceitos sobre os métodos de ordenação externa vistos em sala. Características de seus desempenhos serão avaliadas experimentalmente. Deve-se considerar os métodos de ordenação balanceada multi-caminhos, ordenação polifásica e ordenação em cascata. Há a liberdade de incluir variações desses ou outros métodos, mas pelo menos os três citados devem fazer parte do trabalho. Há material de referência que explora em profundidade esses e outros métodos de ordenação. Consultas a esta e outras referências complementares podem ajudar.

O trabalho deverá ser entregue em forma de relatório. O código fonte correspondente à implementação deverá ser disponibilizado. O relatório deve conter explicações claras e concisas sobre os seguintes itens:

    Descrição dos métodos estudados.
    Descrição dos experimentos e implementação dos métodos. O código fonte deve estar disponível e compartilhado através de algum portal a partir do qual pode-se compilá-lo e executá-lo de forma remota. Informações para permitir o compartilhamento devem estar presentes de forma clara no relatório. Se for necessário, usar o endereço eletrônico gmalima@gmail.com para permitir acesso ao compartilhamento (não enviar o trabalho para este e-mail).
    Apresentação dos resultados experimentais e discussão sobre os aspectos relevantes observados nos experimentos.
    Considerações finais sobre o trabalho, ressaltando os aspectos mais importantes.
    Todas as fontes bibliográficas utilizadas deverão ser citadas de forma apropriada.

O trabalho pode ser feito em equipe de no máximo três componentes e será avaliado em função do relatório e da implementação. Cada um destes aspectos com pesos iguais. Clareza, correção e precisão das informações contidas no texto do relatório têm peso importante na avaliação. A implementação será avaliada executando o código para exemplos conhecidos. Serão atribuídas nota zero a quaisquer trabalhos com indícios de plágio.

O relatório deverá ser entregue até dia 28/05/2024 e as composições das equipes devem ser informadas até dia 14/04/2024. Tanto o relatório quanto informações sobre a formação de equipes devem ser entregues em local indicado na página da disciplina no Ava Moodle. Após 14/04/2024 não será possível modificar as composições das equipes.
2. Avaliação experimental e apresentação dos resultados

Os métodos de ordenação externa estudados devem ser comparados experimentalmente. Os valores a serem ordenados serão numéricos e gerados aleatoriamente. Como métrica de comparação, deve-se usar a taxa de processamento média em função do número de sequências inicial geradas, como explicado a seguir. Além disso, objetiva-se investigar como o número de sequências iniciais varia em função do tamanho da memória interna.
2.1. Métricas de avaliação

Suponha um arquivo a ser ordenado com n registros e assuma que a memória principal só possui capacidade para m registros e que o sistema só tem capacidade de manter abertos concomitantemente k arquivos. Para todos os métodos, gera-se, antes da fase de intercalação, um número inicial r de sequências ordenadas (runs). Para tanto, usa-se o método visto em sala conhecido como seleção natural, que faz uso de uma heap mínima (assume-se aqui ordem ascendente de chaves dos registros). Os registros a serem ordenados serão representados por valores inteiros, que devem ser gerados aleatoriamente durante a fase de geração das sequências iniciais.

Para medir o esforço que cada algoritmo faz para intercalar as sequências iniciais, define-se a taxa de processamento α(r), calculada da seguinte forma:
α(r)=nuˊmero total de operac¸o˜es de escrita sobre os registros do arquivonuˊmero total de registrosα(r)=nuˊmero total de registrosnuˊmero total de operac¸​o˜es de escrita sobre os registros do arquivo​

Desta forma, para uma sequência arbitrária de registros e k arquivos abertos, gera-se as r sequências iniciais. Note que o número registros a serem considerados para valores fixos de r e k não é constante. Considere os valores de k = 4, 6, ..., 12 e m = 3, 15, ..., 60. Os valores de r serão aqueles contidos no conjunto R = {i × j ≤ 5000|i = 1, 2, ..., 10; j = 10, 20, ..., 1000}. O valor de n é aquele que foi suficiente para gerar as r sequências iniciais.

Recomenda-se que o cálculo de α(r) considere ao menos 10 repetições para cada valor de r e k. Outra observação relevante está relacionada à geração dos valores a serem ordenados. Como o valor de n não será determinado de antemão, recomenda-se que valores numéricos aleatórios sejam gerados até que as r sequências iniciais sejam construídas. Por fim, observar que a disposição das sequências iniciais deverá estar de acordo com cada uma dos algoritmos de ordenação. Por exemplo, na ordenação balanceada, deve-se usar apenas k/2 dos arquivos para conter as sequências iniciais. Para os outros dois métodos de ordenação, estas sequências podem estar contidas em k − 1 arquivos.

Em cada um dos experimentos solicita-se ainda que seja avaliado o tamanho médio das sequências. Sabe-se que o tamanho das sequências iniciais depende de m, pois quanto maior a memória interna, maiores tendem a ser tanto as sequências iniciais quanto aquelas geradas durante o processo de intercalação. Para avaliar o tamanho das sequências iniciais e como estas crescem ao longo da ordenação, considere que há rj sequências na fase j de ordenação, com r0 = r representando as sequências iniciais. Defina o fator β(m, j) da seguinte forma:
β(m,j)=1mrj∑i=1rj∣Si,jm∣β(m,j)=mrj​1​∑i=1rj​​∣Si,jm​∣

Com ∣Si,jm∣∣Si,jm​∣ representando o tamanho da i-ésima sequência gerada na fase j da ordenação quando a capacidade da memória é de m registros.

Como pode ser observado, α(r) mede o esforço do método de ordenação no processo de intercalação, sendo, portanto, uma métrica natural de comparação dos diferentes métodos de ordenação. O fator β(m, j), por sua vez, visa a avaliar a efetividade do método de geração de sequências iniciais (para j = 0) e como o tamanho médio das sequências evoluem durante a ordenação (para j > 0).
