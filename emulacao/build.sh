#!/bin/bash

# Criação do diretório build se não existir
mkdir -p build

# Navegar para o diretório build
cd build

# Configurar e compilar com CMake
cmake ..
make

# Verificar se a compilação foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida. Executável criado: teste"
else
    echo "Erro na compilação."
fi
