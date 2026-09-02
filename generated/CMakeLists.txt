cmake_minimum_required(VERSION 3.10)

project(BanglishCompiler)


# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Compiler source files
add_executable(BanglishCompiler

    src/main.cpp

    src/Token.cpp
    src/Lexer.cpp

)