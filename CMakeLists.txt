cmake_minimum_required(VERSION 3.22)

project(ShouldaCouldaWoulda)

add_executable(gen
    generator/src/main.cpp
    generator/src/generator.cpp
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)
