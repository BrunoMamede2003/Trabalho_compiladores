cmake_minimum_required(VERSION 3.10)
project(futz_compiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adiciona diretórios de include
include_directories(/usr/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Adiciona os arquivos do projeto
file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(futz ${SOURCES})