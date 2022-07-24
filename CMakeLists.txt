cmake_minimum_required(VERSION 3.22)
project(DBMS)

set(CMAKE_CXX_STANDARD 14)

add_executable(DBMS main.cpp implementations/Table.cpp headers/Table.h headers/Constants.h implementations/Constants.cpp implementations/BPTree.cpp headers/BPTree.h implementations/RecordPtr.cpp headers/RecordPtr.h implementations/BPTreeNode.cpp headers/BPTreeNode.h implementations/TypeWrapper.cpp headers/TypeWrapper.h implementations/NodeKey.cpp headers/NodeKey.h)
