cmake_minimum_required(VERSION 3.13)
project(c_compiler)

set(CMAKE_CXX_STANDARD 11)

add_executable(test test_lexier.cpp ../include/lexier.h ../include/token.h
../src/lexier.cpp)
