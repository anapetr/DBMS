//
// Created by Ana Petrova on 19.07.22.
//

#include "../headers/DataBase.h"
#include <iostream>
#include <cstdint>
#include <filesystem>

void DataBase::dropTable(const std::string &tableName) {
    std::string pathToDelete = getTable(tableName).getTablePath();
    std::error_code errorCode;
    if (!std::__fs::filesystem::remove_all(pathToDelete, errorCode))
        throw logic_error(errorCode.message());

    fTables.erase(tableName);
    save();
}

Table &DataBase::getTable(const std::string &name) {
    if (tables.find(name) == tables.end())
        throw invalid_argument("There is no such table!");

    return tables[name];
}