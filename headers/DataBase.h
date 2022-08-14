#ifndef DBMS_DATABASE_H
#define DBMS_DATABASE_H

#include <string>
#include <unordered_map>
#include <vector>
#include "DBTable.h"
#include "../utils/FileUtils.h"

class DataBase {
public:
    DataBase(const std::string &dataBasePath, const std::string &DBName) : dataBasePath(dataBasePath), DBName(DBName) {
        FileUtils::createFolder(dataBasePath);
        saveDBInFile();
    }

    DataBase(std::ifstream &in) {
        FileUtils::readString(in, this->dataBasePath);
        FileUtils::readString(in, this->DBName);

        int tablesCount = 0;
        in.read((char *)&tablesCount, sizeof(tablesCount));
        for (int i = 0; i < tablesCount; i++) {
            std::string tableName;
            std::string tablePath;

            FileUtils::readString(in, tableName);
            FileUtils::readString(in, tablePath);

            std::ifstream table(tablePath, std::ios::binary);

            if (!table.is_open()) {
                throw std::invalid_argument(
                        "Possible corruption : Table with name " + tableName + " could not be open.");
            }
            tables.insert({tableName, DBTable(table)});
        }
        in.close();
    }

    DBTable &getTable(const std::string &tableName) {
        if (tables.find(tableName) == tables.end())
            throw std::invalid_argument("There is no such table!");

        return tables.at(tableName);
    }

    int getTablesCount() const {
        return tables.size();
    }

    void saveDBInFile() const {
        std::ofstream out(dataBasePath + DBName + ".bin", std::ios::binary);
        if (!out.is_open())
            throw std::runtime_error("Can not save Database. There was a problem with opening the file for saving");

        FileUtils::writeString(out, dataBasePath);
        FileUtils::writeString(out, DBName);

        int tablesCount = getTablesCount();
        out.write((char *) &tablesCount, sizeof(tablesCount));
        for (const std::pair<std::string, DBTable> &table: tables) {
            FileUtils::writeString(out, table.first);
            FileUtils::writeString(out, table.second.getTablePath());
        }

        out.close();
    }

    void createTable(const std::string &folderPath, const std::string &tableName, std::vector<std::string> &columns,
                     std::unordered_map<std::string, std::string> &nameTypeOfColumns,
                     const std::string &nameOfIndexedColumn = "", int maxRecordsPerPage = 512) {
        if (tables.find(tableName) != tables.end())
            throw std::invalid_argument("There is already a table with this name in the system");

        DBTable table(folderPath, tableName, nameOfIndexedColumn, maxRecordsPerPage, columns, nameTypeOfColumns);
        tables[tableName] = table;
        saveDBInFile();
    }

    void dropTable(const std::string &tableName) {
        std::string pathToDelete = getTable(tableName).getTablePath();
        try {
            this->getTable(tableName);
        }
        catch (const std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }

        tables.erase(tableName);
        saveDBInFile();
    }

    void insert(const std::string &tableName,
                std::vector<std::unordered_map<std::string, TypeWrapper>> nameValueOfColumns) {
        for (size_t i = 0; i < nameValueOfColumns.size(); i++) {
            getTable(tableName).insert(nameValueOfColumns[i]);
        }
        saveDBInFile();
    }

    int remove(const std::string &tableName, DBQuery &query) {
        int deletedRecords = getTable(tableName).remove(query);
        saveDBInFile();
        return deletedRecords;
    }

    void listTables() const {
        size_t tablesCount = getTablesCount();
        if (tablesCount == 0) {
            std::cout << "There are no tables in the database" << std::endl;
            return;
        }

        if (tablesCount == 1) {
            std::cout << "There is 1 table in the database" << std::endl;
        } else {
            std::cout << "There are " << tablesCount << " tables in the database" << std::endl;
        }

        for (const std::pair<std::string, DBTable> table: tables) {
            std::cout << table.first << " ";
        }
    }

    void tableInfo(const std::string &tableName) const {
        if (tables.find(tableName) == tables.end()) {
            throw std::invalid_argument("There is no table with name " + tableName);
        }
        tables.at(tableName).tableInfo();
    }

    std::string getDBPath() {
        return dataBasePath;
    }

private:
    std::string dataBasePath;
    std::string DBName;
    std::unordered_map<std::string, DBTable> tables;
};

#endif //DBMS_DATABASE_H
