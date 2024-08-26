# cmake_minimum_required(VERSION 3.10)
# project(ExternalSortingAlgorithms)

# # Definir o padrão C++20
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Adicionar o diretório de cabeçalhos
# include_directories(include)

# # Adicionar os arquivos fonte
# file(GLOB SOURCES "src/*.cpp")

# # Criar o executável
# add_executable(executavel ${SOURCES})

# # Opcional: Definir flags de compilação
# target_compile_options(executavel PRIVATE -Wall -Wextra)

# project(example_matplotlibcpp)
# cmake_minimum_required(VERSION 3.12)

# find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

# include_directories(${Python3_INCLUDE_DIRS} include)
# add_executable(executavel src/main.cpp)
# target_link_libraries(executavel ${Python3_LIBRARIES})

cmake_minimum_required(VERSION 3.12)
project(ExternalSortingAlgorithms)

# Definir o padrão C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adicionar o diretório de cabeçalhos
include_directories(include)

# Adicionar os arquivos fonte
file(GLOB SOURCES "src/*.cpp")

# Encontrar e configurar o ambiente Python necessário para o matplotlibcpp
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

# Incluir diretórios do Python e bibliotecas necessárias
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})

# Criar o executável único
add_executable(executavel ${SOURCES})

# Linkar as bibliotecas Python ao executável
target_link_libraries(executavel ${Python3_LIBRARIES})

# Definir flags de compilação opcionais
target_compile_options(executavel PRIVATE -Wall -Wextra)
