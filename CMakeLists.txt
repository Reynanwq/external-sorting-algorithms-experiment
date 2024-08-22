cmake_minimum_required(VERSION 3.10)
project(ExternalSortingAlgorithms)

# Definir o padrão C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adicionar o diretório de cabeçalhos
include_directories(include)

# Adicionar os arquivos fonte
file(GLOB SOURCES "src/*.cpp")

# Criar o executável
add_executable(executavel ${SOURCES})

# Opcional: Definir flags de compilação
target_compile_options(executavel PRIVATE -Wall -Wextra)
