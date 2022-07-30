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
    void loadAndExecuteCommand(const std::string& command);
    void splitCommand();
    size_t getSymbolCountInCommand(char symbol, const std::string& command);
    std::string getWordAtIndex(std::string text, size_t index, char delimiter);

    CommandType getCommandType(std::string command) const;


private:
    bool isDistinct = false;
    std::string orderBy;
    std::string input;
    std::vector<std::string> commands;
    //CommandExecutor commandExecutor;
};