#include "../headers/DataBase.h"
#include <iostream>
#include <cstdint>
#include <filesystem>

DataBase::DataBase(std::ifstream& in){
    FileUtils::readString(in, this->dataBasePath);

    size_t tablesCount = 0;
    in.read((char*)tablesCount, sizeof(tablesCount));
    for(size_t i = 0; i < tablesCount; i++) {
        std::string tableName;
        std::string tablePath;

        FileUtils::readString(in, tableName);
        FileUtils::readString(in, tablePath); //"(...).bin"

        std::ifstream table(tablePath, std::ios::binary);

        if(!table.is_open()) {
            throw std::invalid_argument("Possible corruption : Table with name " + tableName + " could not be open.");
        }

        tables.insert({ tableName, DBTable(table)});
    }

    in.close();
}

DBTable &DataBase::getTable(const std::string &tableName) {
    if (tables.find(tableName) == tables.end())
        throw std::invalid_argument("There is no such table!");

    return tables.at(tableName);
}

size_t DataBase::getTablesCount() const{
    return tables.size();
}

void DataBase::savDBInFile() const {
    std::ofstream out(dataBasePath, std::ios::binary);
    if (!out.is_open())
        //TODO what kind of exception
        throw std::runtime_error("Can not save Database. There was a problem with opening the file for saving");

    FileUtils::writeString(out, dataBasePath);

    size_t tablesCount = getTablesCount();
    out.write((char*)&tablesCount, sizeof(tablesCount));
    for (const std::pair<std::string, DBTable>& table : tables)
    {
        FileUtils::writeString(out, table.first);
        FileUtils::writeString(out, table.second.getTablePath());
    }

    out.close();
}

void DataBase::dropTable(const std::string &tableName) {
    std::string pathToDelete = getTable(tableName).getTablePath();

    try {
        this->getTable(tableName);
    }
    catch(const std::invalid_argument& e){
        std::cout << e.what() << std::endl;
    }

    tables.erase(tableName);
    savDBInFile();
}

void DataBase::listTables() const {
    size_t tablesCount = getTablesCount();
    if (tablesCount == 0) {
        std::cout << "There are no tables in the database" << std::endl;
        return;
    }

    if (tablesCount == 1) {
        std::cout << "There is 1 table in the database" << std::endl;
    } else {
        std::cout << "There are " << tablesCount <<" tables in the database" << std::endl;
    }

    for (const std::pair<std::string, DBTable> table : tables) {
        std::cout << table.first << " ";
    }
}