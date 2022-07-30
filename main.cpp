#include <iostream>

#include <vector>
#include <string>
#include <iostream>

#include "./headers/CommandParser.h"

int main() {
    CommandParser cp;
    std::string str = "Insert INTO Sample";
    std::string result = cp.getWordAtIndex(str, 2, ' ');
    std::cout << result << std::endl;

    std::string command = "(ID:Int, Name:String, Value:Int)";
    std::string schema = StringUtils::stringBetweenTwoCharacters(command, "(", ")");
    std::cout << schema << std::endl;

    std::string word = "djajdaA0";
    std::string wordToUpper = StringUtils::toUpper(word);
    std::cout << wordToUpper << std::endl;
    return 0;
}
