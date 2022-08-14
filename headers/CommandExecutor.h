#ifndef DBMS_COMMANDEXECUTOR_H
#define DBMS_COMMANDEXECUTOR_H

#include "./CommandParser.h"
#include "./DataBase.h"
#include "../utils/DBUtils.h"

class CommandExecutor {
public:
    void createTable(CommandParser &commandParser, DataBase &dataBase);

    void dropTable(CommandParser &commandParser, DataBase &dataBase);

    void listTables(DataBase &dataBase);

    void tableInfo(CommandParser &commandParser, DataBase &dataBase);

    void insert(CommandParser &commandParser, DataBase &dataBase);

    void select(CommandParser &commandParser, DataBase &dataBase);

    void remove(CommandParser &commandParser, DataBase &dataBase);

    void save(DataBase &dataBase);

private:
    void printRecords(std::vector<Record>& records, std::vector<std::string>& selectedColumns, std::unordered_map<std::string, int> indexOfColumns) const;

    std::unordered_map<std::string, std::string> setNameTypeOfColumns(std::string columnsInfo, std::vector<std::string>& nameOfColumns);

    std::vector<std::unordered_map<std::string, TypeWrapper>> setNameValueOfColumns(std::string values, std::unordered_map<std::string, std::string>& nameTypeOfColumns, std::unordered_map<int, std::string>& indexOfColumns);

    std::unordered_map<std::string, int> getLongestWordPerCol(std::vector<Record>& records, std::vector<std::string>& selectedColumns, std::unordered_map<std::string, int> colIndex) const;

    int getLongestContentAtCol(int columnIndex, std::vector<Record>& records) const;

    void printColumnsInfo(std::vector<std::string> &selectedColumns, std::unordered_map<std::string, int> &longestWordsPerCol) const;
};

#endif //DBMS_COMMANDEXECUTOR_H
