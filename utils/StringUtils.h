#ifndef DBMS_STRINGUTILS_H
#define DBMS_STRINGUTILS_H

#include <vector>
#include <string>
#include <iostream>
#include <cctype>

class StringUtils {
public:
    static std::vector<std::string> splitLineByDelimiter(std::string line, const std::string& delimeter) {
        std::vector<std::string> result;

        int position = 0;

        if (line.find(delimeter) == std::string::npos && !line.empty()) {
            result.push_back(line);
            return result;
        }

        while ((position = line.find(delimeter)) != std::string::npos) {
            result.push_back(line.substr(0, position));
            line.erase(0, position + delimeter.length());

            if (line.find(delimeter) == std::string::npos) {
                result.push_back(line);
                return result;
            }
        }
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

    static bool isInteger(const std::string& source) {
        if (source.empty()) {
            return false;
        }

        bool isInteger = true;
        for (int i = 0; i < source.size(); i++) {
            if (source[0] == '-') {
                continue;
            }

            if (!isdigit(source[i])) {
                isInteger = false;
            }
        }
        return isInteger;
    }

    static bool isValidString(const std::string& source) {
        if (source.size() < 2 || source.empty() || source[0] != '"' || source[source.size() - 1] != '"') {
            return false;
        }
        return true;
    }

    static std::string& trim(std::string& source) {
        if (source.empty()) {
            return source;
        }

        while (source[0] == ' ') {
            source.erase(source.begin());
        }

        int end = 0;
        if(!source.empty()) {
            end = source.size() - 1;
        }

        while (source[end] == ' ') {
            source.erase(source.begin() + end);
            --end;
        }
        return source;
    }

    static void removeEmptyStringsInVector(std::vector<std::string>& vector) {
        for (size_t i = 0; i < vector.size(); i++) {
            if (vector[i].empty()) {
                vector.erase(vector.begin() + i);
                i--;
            }
        }
    }

    static bool isCorrectColumnType(const std::string& colType, const std::string& actualInput) {
        if ((colType == "Integer" || colType == "Int" || colType == "integer" || colType == "int" || colType == "INTEGER" || colType == "INT") && isInteger(actualInput)) {
            return true;
        } else if ((colType == "String" || colType == "string" || colType == "STRING") && isValidString(actualInput)) {
            return true;
        }
        return false;
    }
};
#endif //DBMS_STRINGUTILS_H
