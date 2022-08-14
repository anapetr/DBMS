cmake_minimum_required(VERSION 3.22)
project(DBMS)

set(CMAKE_CXX_STANDARD 14)

add_executable(DBMS main.cpp
        headers/Application.h
        headers/BPlusTree.h
        headers/BPlusTreeNode.h
        headers/CommandParser.h
        headers/CommandType.h
        headers/DataBase.h
        headers/DBPage.h
        headers/DBQuery.h
        headers/DBTable.h
        headers/IntegerType.h
        headers/InternalQuery.h
        headers/NodeKey.h
        headers/Object.h
        headers/ObjectType.h
        headers/Record.h
        headers/RecordPtr.h
        headers/StringType.h
        headers/TypeWrapper.h
        headers/CommandExecutor.h
        implementations/Application.cpp
        implementations/BPlusTree.cpp
        implementations/BPTreeNode.cpp
        implementations/CommandParser.cpp
        implementations/DBQuery.cpp
        implementations/DBTable.cpp
        implementations/TypeWrapper.cpp
        implementations/CommandExecutor.cpp
        utils/DBUtils.h
        utils/FileUtils.h
        utils/StringUtils.h)
