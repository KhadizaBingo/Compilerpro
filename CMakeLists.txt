cmake_minimum_required(VERSION 3.16)
project(BanglishCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BanglishCompiler
    src/main.cpp
    src/Token.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/AST.cpp
    src/SemanticAnalyzer.cpp
    src/CodeGenerator.cpp
)

target_include_directories(BanglishCompiler PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
