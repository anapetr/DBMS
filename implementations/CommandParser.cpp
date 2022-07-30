#include "../headers/CommandParser.h"
#include "../utils/DBUtils.h"

//TODO - we need the database;
void CommandParser::loadAndExecuteCommand(const std::string &command) {
    if (getSymbolCountInCommand('{', command) != getSymbolCountInCommand('}', command)) {
        throw std::invalid_argument("Invalid command, check the number of curly braces");
    }

    if (getSymbolCountInCommand('(', command) != getSymbolCountInCommand(')', command)) {
        throw std::invalid_argument("Invalid command, check the number of braces");
    }

    if (getSymbolCountInCommand('\"', command) % 2 != 0) {
        throw std::invalid_argument("Invalid command, check the number of quotes");
    }

//    input = command;

    std::string firstCommand = getWordAtIndex(command, 1, ' ');
    CommandType commandType = getCommandType(firstCommand);

    switch (commandType) {
        case CommandType::CREATE_TABLE:
            try {
                std::string tableName = getWordAtIndex(command, 2, ' ');
                std::string schema = StringUtils::stringBetweenTwoCharacters(command, "(", ")");
                std::unordered_map<std::string, std::string> nameTypeOfColumns = DBUtils::getNameAndTypesOfColumns(schema);

            } catch (...) {

            }
        case CommandType::DROP_TABLE:
            try {
                std::string tableName = getWordAtIndex(command, 2, ' ');


            } catch (...) {

            }
        case CommandType::LIST_TABLES:
            try {

            } catch (...) {

            }
        case CommandType::TABLE_INFO:
            try {
                std::string tableName = getWordAtIndex(command, 2, ' ');


            } catch (...) {

            }
        case CommandType::INSERT:
            try {
                std::string tableName = getWordAtIndex(command, 2, ' ');
                std::string schema = StringUtils::stringBetweenTwoCharacters(command, "(", ")");
                std::unordered_map<std::string, std::string> nameTypeOfColumns = DBUtils::getNameAndTypesOfColumns(schema);

            } catch (...) {

            }
        case CommandType::REMOVE:
            try {
                std::string tableName = getWordAtIndex(command, 2, ' ');
                std::string schema = StringUtils::stringBetweenTwoCharacters(command, "(", ")");
                std::unordered_map<std::string, std::string> nameTypeOfColumns = DBUtils::getNameAndTypesOfColumns(schema);

            } catch (...) {

            }
        case CommandType::SELECT:
            try {
                std::string tableName = getWordAtIndex(command, 2, ' ');
                std::string schema = StringUtils::stringBetweenTwoCharacters(command, "(", ")");
                std::unordered_map<std::string, std::string> nameTypeOfColumns = DBUtils::getNameAndTypesOfColumns(schema);

            } catch (...) {

            }
        case CommandType::EXIT:
            try {
                std::string tableName = getWordAtIndex(command, 2, ' ');
                std::string schema = StringUtils::stringBetweenTwoCharacters(command, "(", ")");
                std::unordered_map<std::string, std::string> nameTypeOfColumns = DBUtils::getNameAndTypesOfColumns(schema);

            } catch (...) {

            }
        case CommandType::NONE:
            break;
    }
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

std::string CommandParser::getWordAtIndex(std::string text, size_t index, char delimiter) {
    size_t currIndex = 1;
    std::string currWord = "";
    int size = text.size();
    for (size_t i = 0; i < size; i++) {
        if (text[i] == delimiter || i == size - 1) {
            if (currIndex == index) {
                return i == size - 1 ? currWord + text[i] : currWord;
            } else {
                currWord = "";
                currIndex++;
            }
        } else {
            currWord += text[i];
        }
    }

    //TODO - throw exeption
}

CommandType CommandParser::getCommandType(std::string command) const{
    std::string commandToUpper = StringUtils::toUpper(command);

    if (commandToUpper == "CREATETABLE") {
        return CommandType::CREATE_TABLE;
    }
    else if (commandToUpper == "DROPTABLE"){
        return CommandType::DROP_TABLE;
    }
    else if (commandToUpper == "LISTTABLES"){
        return CommandType::LIST_TABLES;
    }
    else if (commandToUpper == "TABLEINFO"){
        return CommandType::TABLE_INFO;
    }
    else if (commandToUpper == "INSERT"){
        return CommandType::INSERT;
    }
    else if (commandToUpper == "REMOVE"){
        return CommandType::REMOVE;
    }
    else if (commandToUpper == "SELECT"){
        return CommandType::SELECT;
    }
    else if (commandToUpper == "EXIT") {
        return CommandType::EXIT;
    }

    return CommandType::NONE;
}