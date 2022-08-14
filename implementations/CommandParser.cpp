#include "../headers/CommandParser.h"

void CommandParser::loadCommand(const std::string &command) {
    if (getSymbolCountInCommand('{', command) != getSymbolCountInCommand('}', command)) {
        throw std::invalid_argument("Invalid command, check the number of curly braces");
    }

    if (getSymbolCountInCommand('(', command) != getSymbolCountInCommand(')', command)) {
        throw std::invalid_argument("Invalid command, check the number of braces");
    }

    if (getSymbolCountInCommand('\"', command) % 2 != 0) {
        throw std::invalid_argument("Invalid command, check the number of quotes");
    }

    clearCommand();
    input = command;
    splitCommand();

    if (input.empty() || commands.empty()) {
        throw std::invalid_argument("Invalid number of commands");
    }

    isDistinct = std::find(commands.begin(), commands.end(), "DISTINCT") != commands.end();

    if (std::find(commands.begin(), commands.end(), "ORDER") != commands.end()) {
        for (int i = 0; i < commands.size() - 1; i++) {
            if (commands[i] == "ORDER" && commands[i + 1] == "BY") {
                orderBy = commands[i + 2];
            }
        }
    }

    if (std::find(commands.begin(), commands.end(), "WHERE") != commands.end()) {
        for (int i = 0; i < commands.size(); i++) {
            int helperIndex = i;
            if (commands[i] == "WHERE") {
                i++;
                while (i < commands.size() &&
                       (commands[i] != "ORDER" && commands[i] != "BY" && commands[i] != "DISTINCT")) {
                    commands[helperIndex] += " " + commands[i];
                    i++;
                }
            }
        }
    }
}

void CommandParser::splitCommand() {
    int i = 0;
    while (input[i]) {
        if (input[i] == ' ') {
            while (input[i] == ' ') {
                i++;
            }
        } else if (input[i] == '{') {
            int helperIndex = i;
            while (input[i] != '}') {
                i++;
            }
            i++;
            commands.push_back(input.substr(helperIndex, i - helperIndex));
        } else if (input[i] == '(') {
            int helperIndex = i;
            while (input[i] != ')') {
                i++;
            }
            i++;
            commands.push_back(input.substr(helperIndex, i - helperIndex));
        } else if (input[i] == '"') {
            int helperIndex = i;
            while (input[i] != '"') {
                i++;
            }
            i++;
            commands.push_back(input.substr(helperIndex, i - helperIndex));
        } else {
            int helperIndex = i;
            while (isalpha(input[i]) || isdigit(input[i]) || input[i] == '*' || input[i] == '>' || input[i] == '<' ||
                   input[i] == '='
                   || input[i] == '!' || input[i] == ',' || input[i] == '.') {
                i++;
            }
            commands.push_back(input.substr(helperIndex, i - helperIndex));
        }
    }
}


int CommandParser::getSymbolCountInCommand(char symbol, const std::string &command) {
    size_t count = 0;
    for (int i = 0; i < command.size(); i++) {
        if (command[i] == symbol) {
            count++;
        }
    }
    return count;
}

const std::string &CommandParser::getWordAtIndex(int index) const {
    if (index >= commands.size()) {
        throw std::out_of_range("There are not enough arguments.");
    }

    return commands[index];
}

CommandType CommandParser::getCommandType(std::string command) const {
    std::string commandToUpper = StringUtils::toUpper(command);

    if (commandToUpper == "CREATETABLE") {
        return CommandType::CREATE_TABLE;
    } else if (commandToUpper == "DROPTABLE") {
        return CommandType::DROP_TABLE;
    } else if (commandToUpper == "LISTTABLES") {
        return CommandType::LIST_TABLES;
    } else if (commandToUpper == "TABLEINFO") {
        return CommandType::TABLE_INFO;
    } else if (commandToUpper == "INSERT") {
        return CommandType::INSERT;
    } else if (commandToUpper == "REMOVE") {
        return CommandType::REMOVE;
    } else if (commandToUpper == "SELECT") {
        return CommandType::SELECT;
    } else if (commandToUpper == "EXIT") {
        return CommandType::EXIT;
    }

    return CommandType::NONE;
}

void CommandParser::clearCommand() {
    commands.clear();
    input.clear();
}

int CommandParser::getSizeOfCommands() const {
    return commands.size();
}

std::string CommandParser::getOrderBy() const {
    return orderBy;
}

bool CommandParser::getIsDistinct() const {
    return isDistinct;
}