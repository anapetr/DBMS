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