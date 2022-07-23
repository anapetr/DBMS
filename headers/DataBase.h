//
// Created by Ana Petrova on 19.07.22.
//

#ifndef DBMS_DATABASE_H
#define DBMS_DATABASE_H

#include <string>
#include <unordered_map>
#include <vector>

class DataBase {
    void createTable(const std::string& path,
                     const std::string& tableName,
                     std::unordered_map<std::string,
                     std::string>& colNameType,
                     std::vector<std::string>& colNames,
                     const std::string primaryKey,
                     int maxRecordsPerPage);
};


#endif //DBMS_DATABASE_H
