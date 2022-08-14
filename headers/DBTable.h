#ifndef DBMS_DBTABLE_H
#define DBMS_DBTABLE_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "BPlusTree.h"
#include "DBPage.h"
#include "DBQuery.h"
#include "../utils/DBUtils.h"

class DBTable {
public:
    DBTable();

    DBTable(const std::string &tablePath,
            const std::string &tableName,
            const std::string &nameOfIndexedColumn,
            int maxRecordsPerPage,
            std::vector<std::string> columns,
            std::unordered_map<std::string, std::string> &nameTypeOfColumns);

    DBTable(std::ifstream &in);

    void saveTableToFile();

    void indexColumns(std::vector<std::string> columns);

    void indexTable(std::string nameOfIndexedColumn);

    void checkNewRecordsAgainstTypes(std::unordered_map<std::string, TypeWrapper> &newRecord);

    DBPage addNewPage();

    std::vector<int> getColumnsIndexesByName(const std::vector<std::string> &columnsNames);

    bool checkRecordExists(const std::string &colName, const TypeWrapper &colValue);

    std::vector<Record> getRecords(std::vector<RecordPtr> &recordPtrs);

    void parseInfoForColumns(std::vector<std::string> &columns);

    //getters
    std::vector<std::string> getOrderedColumnNames() const;

    std::unordered_map<std::string, std::string> getNameTypeOfColumns() const;

    std::string getNameOfIndexedColumn() const;

    std::string getTablePath() const;

    std::unordered_map<std::string, int> getIndexOfColumns() const;

    std::string getInfoForColumns() const;

    //commands
    void insert(std::unordered_map<std::string, TypeWrapper> &newRecord);

    std::vector<Record> select(DBQuery &query);

    std::vector<Record> select(DBQuery &query, const std::string &orderByWhat, bool isDistinct, std::vector<std::string> &selectedColumns);

    int remove(DBQuery &query);

    void orderBy(std::vector<Record> &recordsToOrder, const std::string &orderByWhat);

    std::vector<Record> distinct(std::vector<Record> &recordsToDistinct, std::vector<std::string> &columns);

    void tableInfo() const;

private:
    std::string folderPath; //keeping all pages in this folder
    std::string tableName;
    std::string nameOfIndexedColumn; //default ""
    std::string infoForColumns;
    long sizeOfTableInBytes;
    int maxRecordsInPage;
    int currentPage;
    int columnsCount;
    std::unordered_map<std::string, std::string> nameTypeOfColumns;
    std::unordered_map<std::string, int> indexOfColumns;
    BPlusTree bPlusTree; //Tree for the indexed column
};


#endif //DBMS_DBTABLE_H
