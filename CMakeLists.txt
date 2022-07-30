cmake_minimum_required(VERSION 3.22)
project(DBMS)

set(CMAKE_CXX_STANDARD 14)

add_executable(DBMS main.cpp
        headers/Application.h
        headers/CommandExecutor.h
        headers/CommandParser.h
        headers/DataBase.h
        headers/DBTable.h
        headers/Constants.h
        headers/BPTree.h
        headers/RecordPtr.h
        headers/BPTreeNode.h
        headers/TypeWrapper.h
        headers/NodeKey.h
        implementations/Application.cpp
        implementations/CommandExecutor.cpp
        implementations/CommandParser.cpp
        implementations/DataBase.cpp
        implementations/DBTable.cpp
        implementations/BPTree.cpp
        implementations/RecordPtr.cpp
        implementations/BPTreeNode.cpp
        implementations/TypeWrapper.cpp
        implementations/NodeKey.cpp
        utils/DBUtils.h
        utils/FileUtils.h
        utils/StringUtils.h)
