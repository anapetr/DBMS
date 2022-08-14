#include "../headers/CommandExecutor.h"

void CommandExecutor::createTable(CommandParser &commandParser, DataBase &dataBase) {
    std::string tableName = commandParser.getWordAtIndex(1);
    std::vector<std::string> columns;
    std::unordered_map<std::string, std::string> nameTypeOfColumns = setNameTypeOfColumns(commandParser.getWordAtIndex(2), columns);
    std::string nameOfIndexedColumn = "";
    if (commandParser.getSizeOfCommands() >= 6) {
        nameOfIndexedColumn = commandParser.getWordAtIndex(5);
    }
    dataBase.createTable(dataBase.getDBPath(), tableName, columns, nameTypeOfColumns, nameOfIndexedColumn);
}

void CommandExecutor::dropTable(CommandParser &commandParser, DataBase &dataBase) {
    dataBase.dropTable(commandParser.getWordAtIndex(1));
}

void CommandExecutor::listTables(DataBase &dataBase) {
    dataBase.listTables();
}

void CommandExecutor::tableInfo(CommandParser &commandParser, DataBase &dataBase) {
    dataBase.tableInfo(commandParser.getWordAtIndex(1));
}

void CommandExecutor::insert(CommandParser &commandParser, DataBase &dataBase) {
    std::string tableName = commandParser.getWordAtIndex(2);

    DBTable &table = dataBase.getTable(tableName);
    std::unordered_map<int, std::string> indexOfColumns;
    std::unordered_map<std::string, std::string> nameTypeOfColumns = table.getNameTypeOfColumns();

    int index = 0;
    std::vector<std::string> header = StringUtils::splitLineByDelimiter(table.getInfoForColumns(), ",");
    StringUtils::removeEmptyStringsInVector(header);
    for (const std::string &entry: header) {
        indexOfColumns.insert({index++, entry});
    }

    std::vector<std::unordered_map<std::string, TypeWrapper>> values = setNameValueOfColumns(commandParser.getWordAtIndex(3),
                                                                                        nameTypeOfColumns,
                                                                                        indexOfColumns);
    dataBase.insert(tableName, values);
    std::cout << "Total " << values.size() << " rows inserted." << std::endl;
}

void CommandExecutor::select(CommandParser &commandParser, DataBase &dataBase) {
    std::vector<std::string> selectedColumns = StringUtils::splitLineByDelimiter(commandParser.getWordAtIndex(1), ",");
    std::string tableName = commandParser.getWordAtIndex(3);
    DBTable &table = dataBase.getTable(tableName);
    bool isDistinct = commandParser.getIsDistinct();
    std::string orderBy = commandParser.getOrderBy();

    if (commandParser.getSizeOfCommands() <= 4) {
        DBQuery q("", table.getNameTypeOfColumns(), table.getNameOfIndexedColumn());
        if (selectedColumns.size() == 1 && selectedColumns[0] == "*") {
            selectedColumns = StringUtils::splitLineByDelimiter(dataBase.getTable(tableName).getInfoForColumns(), ",");
            StringUtils::removeEmptyStringsInVector(selectedColumns);
            std::vector<Record> answer = table.select(q, orderBy, isDistinct, selectedColumns);
            printRecords(answer, selectedColumns, table.getIndexOfColumns());
        } else {
            std::vector<Record> answer = table.select(q, orderBy, isDistinct, selectedColumns);
            printRecords(answer, selectedColumns, table.getIndexOfColumns());
        }
    } else {
        DBQuery query(commandParser.getWordAtIndex(4), table.getNameTypeOfColumns(), table.getNameOfIndexedColumn());
        if (selectedColumns.size() == 1 && selectedColumns[0] == "*") {
            selectedColumns = StringUtils::splitLineByDelimiter(dataBase.getTable(tableName).getInfoForColumns(), ",");
            StringUtils::removeEmptyStringsInVector(selectedColumns);
            std::vector<Record> answer = table.select(query, orderBy, isDistinct, selectedColumns);
            printRecords(answer, selectedColumns, table.getIndexOfColumns());
        } else {
            std::vector<Record> answer = table.select(query, orderBy, isDistinct, selectedColumns);
            printRecords(answer, selectedColumns, table.getIndexOfColumns());
        }
    }
}

void CommandExecutor::remove(CommandParser &commandParser, DataBase &dataBase) {
    std::string tableName = commandParser.getWordAtIndex(2);
    DBTable &table = dataBase.getTable(tableName);
    DBQuery query(commandParser.getWordAtIndex(3), table.getNameTypeOfColumns(), table.getNameOfIndexedColumn());
    std::cout << "Total " << dataBase.remove(tableName, query) << " rows removed from " << tableName << std::endl;
}

void CommandExecutor::save(DataBase &dataBase) {
    dataBase.saveDBInFile();
}

void CommandExecutor::printRecords(std::vector<Record> &records, std::vector<std::string> &selectedColumns, std::unordered_map<std::string, int> indexOfColumns) const {
    std::unordered_map<std::string, int> longestWordsPerCol = getLongestWordPerCol(records, selectedColumns,indexOfColumns);
    printColumnsInfo(selectedColumns, longestWordsPerCol);

    for (int i = 0; i < records.size(); i++) {
        std::cout << " | ";
        for (int j = 0; j < selectedColumns.size(); j++) {
            TypeWrapper content = records[i].getValue(indexOfColumns[selectedColumns[j]]);
            if (content.getContent() != nullptr) {
                std::string spaces(
                        selectedColumns[j].size() > longestWordsPerCol[selectedColumns[j]] ? selectedColumns[j].size() -
                                                                                             content.getContent()->size()
                                                                                           :
                        longestWordsPerCol[selectedColumns[j]] - content.getContent()->size(), ' ');
                std::cout << content.getContent()->toString();
                std::cout << spaces;
            }
            std::cout << " | ";
        }
        std::cout << std::endl;
    }
    std::cout << "Total " << records.size() << " records selected." << std::endl;
}


std::unordered_map<std::string, std::string> CommandExecutor::setNameTypeOfColumns(std::string columnsInfo, std::vector<std::string> &nameOfColumns) {
    std::unordered_map<std::string , std::string> colNameType;
    columnsInfo.erase(columnsInfo.begin());
    columnsInfo.pop_back();

    std::vector<std::string> parts = StringUtils::splitLineByDelimiter(columnsInfo, ",");
    for (int i = 0; i < parts.size(); i++) {
        std::vector<std::string> tuple = StringUtils::splitLineByDelimiter(parts[i], ":");
        nameOfColumns.push_back(tuple[0]);
        colNameType.insert({ tuple[0], tuple[1] });
    }
    return colNameType;
}

std::vector<std::unordered_map<std::string, TypeWrapper>> CommandExecutor::setNameValueOfColumns(std::string values, std::unordered_map<std::string, std::string> &nameTypeOfColumns, std::unordered_map<int, std::string> &indexOfColumns) {
    std::vector<std::unordered_map<std::string, TypeWrapper>> result;
    values.erase(values.begin());
    values.pop_back();

    std::vector<std::string> parts = StringUtils::splitLineByDelimiter(values, ", ");
    for (size_t i = 0; i < parts.size(); i++) {
        StringUtils::trim(parts[i]);
        parts[i].erase(parts[i].begin());
        parts[i].pop_back();

        std::vector<std::string> splitRecord = StringUtils::splitLineByDelimiter(parts[i], ",");
        if (splitRecord.size() != nameTypeOfColumns.size()) {
            throw std::invalid_argument(
                    "Passed number of columns and table's number of columns are mismatching. Please check the record you are about to insert.");
        }

        std::unordered_map<std::string, TypeWrapper> colVal;
        for (size_t j = 0; j < splitRecord.size(); j++) {
            std::string colName = indexOfColumns[j];
            std::string colType = nameTypeOfColumns[colName];

            if (StringUtils::isCorrectColumnType(colType, splitRecord[j])) {
                if (colType == "Integer" || colType == "Int" || colType == "integer" || colType == "int" || colType == "INTEGER" || colType == "INT") {
                    colVal.insert({colName, TypeWrapper(stoi(splitRecord[j]))});
                } else if (colType == "String" || colType == "string" || colType == "STRING") {
                    colVal.insert({colName, TypeWrapper(splitRecord[j])});
                }
            } else {
                throw std::invalid_argument("Invalid type for column {" + colName + "} with value {" + splitRecord[j] + "}");
            }
        }
        result.push_back(colVal);
    }
    return result;

}

std::unordered_map<std::string, int> CommandExecutor::getLongestWordPerCol(std::vector<Record> &records, std::vector<std::string> &selectedColumns, std::unordered_map<std::string, int> colIndex) const {
    std::unordered_map<std::string, int> lengthPerCol;
    for (size_t i = 0; i < selectedColumns.size(); i++) {
        lengthPerCol.insert({selectedColumns[i], getLongestContentAtCol(colIndex[selectedColumns[i]], records)});
    }
    return lengthPerCol;
}

int CommandExecutor::getLongestContentAtCol(int columnIndex, std::vector<Record> &records) const {
    int result = 0;
    for (size_t i = 0; i < records.size(); i++) {
        TypeWrapper content = records[i].getValue(columnIndex);
        if (content.getContent() != nullptr) {
            int len = content.getContent()->size();
            if (result < len) {
                result = len;
            }
        }
    }
    return result;
}

void CommandExecutor::printColumnsInfo(std::vector<std::string> &selectedColumns, std::unordered_map<std::string, int> &longestWordsPerCol) const {
    for (int j = 0; j < selectedColumns.size(); j++) {
        std::string spaces;
        if (longestWordsPerCol[selectedColumns[j]] != 0) {
            int numSpaces = longestWordsPerCol[selectedColumns[j]] > selectedColumns[j].size() ?
                            longestWordsPerCol[selectedColumns[j]] - selectedColumns[j].size() : 0;
                spaces = std::string(numSpaces, ' ');
        }
        std::cout << " | " << selectedColumns[j] << spaces;
    }
    std::cout << " |" << std::endl;

    std::cout << " --";
    for (size_t i = 0; i < selectedColumns.size(); i++) {
        for (size_t j = 0; j < longestWordsPerCol[selectedColumns[i]]; j++) {
            std::cout << "-";
        }
        std::cout << "---";
    }
    std::cout << "--" << std::endl;
}


