cmake_minimum_required(VERSION 3.28)
project(paradigms_a5)

set(CMAKE_CXX_STANDARD 17)

add_executable(paradigms_a5 main.cpp
        Interpreter/Interpreter.h
        Interpreter/Interpreter.cpp)
