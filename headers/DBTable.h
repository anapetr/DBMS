//
// Created by Ana Petrova on 21.07.22.
//

#ifndef DBMS_DBTABLE_H
#define DBMS_DBTABLE_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>
class DBTable {
public:
    DBTable(std::ifstream& in);

    std::string getTablePath() const;

    void tableInfo() const;


private:
    std::string tablePath;
    std::string tableName;
    std::string nameOfIndexedColumn; //default "" - maybe vector<std::string> защото може да има няколко индексирани колони
    long sizeOfTableInBytes;
    int maxRecordsInPage;
    int currentPage;

    std::unordered_map<std::string, std::string> nameTypeOfColumns;
   // std::unordered_map>;


    //Не зареждаме изобщо цялата таблица в паметта, дали винаги имаме key?
    //BPlusTree
};


#endif //DBMS_DBTABLE_H
