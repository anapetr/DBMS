//
// Created by Ana Petrova on 19.07.22.
//

#include "../headers/CommandExecutor.h"

void CommandExecutor::executeCommand(const CommandParser &commandParser,
                                     DataBase& dataBase) {
    switch (commandParser.getCommandType()) {
        case CommandType::CREATE_TABLE:
            createTable(commandParser, dataBase);
            break;
        case CommandType::DROP_TABLE:
            dropTable(commandParser, dataBase);
            break;
        case CommandType::LIST_TABLES:
            listTables(dataBase);
            break;
        case CommandType::TABLE_INFO:
            tableInfo(commandParser, dataBase);
            break;
        case CommandType::INSERT:
            insert(commandParser, dataBase);
            break;
        case CommandType::REMOVE:
            remove(commandParser, dataBase);
            break;
        case CommandType::SELECT:
            select(commandParser, dataBase);
            break;
        case CommandType::EXIT:
            save(dataBase);
            std::cout << "Goodbye" << std::endl;
            break;
        default:
            std::cout << "Unrecognized command." << std::endl;
            break;
    }
}

//from engine and function
void CommandExecutor::createTable(const CommandParser &commandParser, DataBase &dataBase) {
    std::string tableName = commandParser.atToken(1);
    std::vector<string> columNames;
    std::unordered_map<string, string> scheme = DBUtils::getNameAndTypesOfColumns(commandParser.atToken(2), columNames);
    string primaryKey = commandParser.size() >= 6 ? commandParser.atToken(5) : "";
//    dataBase.createTable(dbPath, tblName, scheme, colNames, primaryKey);
}