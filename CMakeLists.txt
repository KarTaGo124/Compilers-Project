cmake_minimum_required(VERSION 3.10)
project(KotlinCompiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler
        environment.h
        exp.cpp
        exp.h
        main.cpp
        parser.cpp
        parser.h
        scanner.cpp
        scanner.h
        token.cpp
        token.h
        visitor.cpp
        visitor.h)
