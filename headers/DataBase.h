//
// Created by Ana Petrova on 19.07.22.
//

#ifndef DBMS_DATABASE_H
#define DBMS_DATABASE_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Table.h"
#include "Constants.h"

class DataBase {
public:
    void createTable(const std::string &path,
                     const std::string &tableName,
                     std::unordered_map<std::string, std::string> &colNameType,
                     std::vector<std::string> &colNames,
                     const std::string primaryKey,
                     int maxRecordsPerPage);

    void dropTable(const std::string &tableName);

    Table &getTable(const std::string &name);

private:
    std::string databaseName = Constants::getNameOfDb();
    std::string pathToDatabase = Constants::getPathToDb();
    std::unordered_map<std::string, Table> tables;
};


#endif //DBMS_DATABASE_H
