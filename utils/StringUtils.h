//
// Created by Ana Petrova on 21.07.22.
//

#ifndef DBMS_STRINGUTILS_H
#define DBMS_STRINGUTILS_H

#include <vector>
#include <string>

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
};


#endif //DBMS_STRINGUTILS_H
