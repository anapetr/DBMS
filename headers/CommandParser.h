#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "CommandType.h"
//#include "CommandExecutor.h"
#include "../utils/StringUtils.h"

class CommandParser {
public:
    void loadCommand(const std::string &command);

    void splitCommand();

    int getSymbolCountInCommand(char symbol, const std::string &command);

    const std::string& getWordAtIndex(int index) const;

    CommandType getCommandType(std::string command) const;

    void clearCommand();

    int getSizeOfCommands() const;

    std::string getOrderBy() const;

    bool getIsDistinct() const;

private:
    bool isDistinct = false;
    std::string orderBy;
    std::string input;
    std::vector<std::string> commands;
};