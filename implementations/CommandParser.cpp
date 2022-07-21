//
// Created by Ana Petrova on 19.07.22.
//

#include "../headers/CommandParser.h"

void CommandParser::loadCommand(const string &command) {
    if (getSymbolCountInCommand('{', command) != getSymbolCountInCommand('}', command)) {
        throw invalid_argument("Invalid command, check the number of curly braces");
    }

    if (getSymbolCountInCommand('(', command) != getSymbolCountInCommand(')', command)) {
        throw invalid_argument("Invalid command, check the number of braces");
    }

    if (getSymbolCountInCommand('\"', command) % 2 != 0) {
        throw invalid_argument("Invalid command, check the number of quotes");
    }


}

void CommandParser::splitCommand(){

}


size_t CommandParser::getSymbolCountInCommand(char symbol, const std::string &command) {
    size_t count = 0;
    for (int i = 0; i < command.size(); i++) {
        if (command[i] == symbol) {
            count++;
        }
    }
    return count;
}

CommandType CommandParser::getCommandType() const {
    string cmd = sh::toUpper(commands[0]);

    if (cmd == "CREATETABLE") {
        return CommandType::CREATE_TABLE;
    } else if (cmd == "DROPTABLE") {
        return CommandType::DROP_TABLE;
    } else if (cmd == "LISTTABLES") {
        return CommandType::LIST_TABLES;
    } else if (cmd == "TABLEINFO") {
        return CommandType::TABLE_INFO;
    } else if (cmd == "INSERT") {
        return CommandType::INSERT;
    } else if (cmd == "REMOVE") {
        return CommandType::REMOVE;
    } else if (cmd == "SELECT") {
        return CommandType::SELECT;
    } else if (cmd == "EXIT")
        return CommandType::EXIT;

    //return CommandType::NONE;
}