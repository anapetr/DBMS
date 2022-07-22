cmake_minimum_required(VERSION 3.22)
project(DBMS)

set(CMAKE_CXX_STANDARD 14)

add_executable(DBMS main.cpp implementations/Table.cpp headers/Table.h headers/Constants.h implementations/Constants.cpp)
