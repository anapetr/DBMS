#ifndef DBMS_STRINGUTILS_H
#define DBMS_STRINGUTILS_H

#include <vector>
#include <string>
#include <iostream>
#include <cctype>

class StringUtils {
public:
    static std::vector<std::string> splitLineByDelimiter(std::string line, char delimiter) {
//        char* helper = &line[0];
//        std::vector<std::string> vector;
//        while (*helper != '\0') {
//            std::string builder;
//            while (*helper != delimiter && *helper != '\0') {
//                builder.push_back(*helper);
//                helper++;
//            }
//            vector.push_back(builder);
//            helper++;
//        }
//
//        return vector;
        std::vector<std::string> result;
        std::string current = "";

        int length = line.size();
        for (size_t i = 0; i < length; ++i) {
            if(line[i] != delimiter) {
                current+=line[i];
            } else {
                result.push_back(current);
                current = "";
            }
        }
        result.push_back(current);
        return result;
    }

    static std::string toUpper(const std::string& str){
        std::string result;
        for (size_t i = 0; i < str.size(); i++){
            if (str[i] >= 'a' && str[i] <= 'z') {
                result += (char)(str[i] - 32);
            } else {
                result += str[i];
            }
        }
        return result;
    }

    static std::string stringBetweenTwoCharacters(std::string str, std::string startDelimiter, std::string stopDelimiter) {
        unsigned first = str.find(startDelimiter);
        unsigned last = str.find_last_of(stopDelimiter);
        return str.substr (first + 1,last - 1);
    }
};


#endif //DBMS_STRINGUTILS_H
