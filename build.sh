#!/bin/bash

# Precisa dar permissão pro arquivo rodar, execute o comando abaixo antes de tudo
# chmod +x build.sh

# Compilador e flags
CXX=g++

# Se quiser mais alerts use essas flags
CXXFLAGS="-Wall -Wextra -std=c++20"

# Arquivos de origem
SRCS="main.cpp funcoes_gerais.cpp funcoes_secundarias.cpp funcoes_ordenacao_balanceada.cpp funcoes_ordenacao_polifasica.cpp funcoes_ordenacao_cascata.cpp"

# Nome do executável
TARGET="teste.exe"

# Compilar os arquivos - Escolha se quer com flags ou não
$CXX $CXXFLAGS $SRCS -o $TARGET
# $CXX $SRCS -o $TARGET

# Verificar se a compilação foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida. Executável criado: $TARGET"
else
    echo "Erro na compilação."
fi
