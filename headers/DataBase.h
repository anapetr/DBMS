//
// Created by Ana Petrova on 19.07.22.
//

#ifndef DBMS_DATABASE_H
#define DBMS_DATABASE_H

#include <string>
#include <unordered_map>
#include <vector>
#include "DBTable.h"
#include "../utils/FileUtils.h"

class DataBase {
public:
    DataBase(std::ifstream& in);

    DBTable &getTable(const std::string &tableName);

    size_t getTablesCount() const;

    void savDBInFile() const;

    void createTable(const std::string &path,
                     //const std::string &tableName,
                     std::unordered_map<std::string, std::string> &colNameType,
                     std::vector<std::string> &colNames,
                     const std::string primaryKey,
                     int maxRecordsPerPage);

    void dropTable(const std::string &tableName);

    void insert(const std::string& tableName); //something more

    void remove(const std::string& tableName); //something more

    void listTables() const;

    void tableInfo(std::string& tableName) const;


private:
    //std::string databaseName;
    std::string dataBasePath; //we need it for save;
    std::unordered_map<std::string, DBTable> tables;
};

#endif //DBMS_DATABASE_H
