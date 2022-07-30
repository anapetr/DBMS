#include "../headers/DBTable.h"

DBTable::DBTable(std::ifstream& in) {

}

std::string DBTable::getTablePath() const { //in order to access the right pages
    return tablePath + tableName + ".bin";
}


//TODO - ;
void DBTable::tableInfo() const {
    std::cout << "Table " << tableName << " : ";
    std::cout << "(";
    for (const std::pair<std::string, std::string> column: nameTypeOfColumns) {
        std::string columnName = column.first;
        std::string columnType = column.second;
        if (columnName == nameOfIndexedColumn) {
            std::cout << columnName << ":" << column.second << ", Indexed;";
        } else {
            std::cout << columnName << ":" << column.second << ";";
        }
    }
    std::cout << ")";
}