#ifndef DBMS_DBUTILS_H
#define DBMS_DBUTILS_H

#include <string>
#include <unordered_map>
#include <vector>
#include "StringUtils.h"

class DBUtils {
public:
    //TODO - check why we need colNames
    static std::unordered_map<std::string, std::string> getNameAndTypesOfColumns(std::string schema/*, std::vector<std::string> colNames*/) {
        std::unordered_map<std::string, std::string> result;

        //remove first and last elements, which are ()
        schema.erase(schema.begin());
        schema.pop_back();

        std::vector<std::string> separated = StringUtils::splitLineByDelimiter(schema, ',');

        for (std::string str: separated) {
            std::pair<std::string, std::string> nameTypePair;
            std::vector<std::string> pair = StringUtils::splitLineByDelimiter(str, ':');

            //in case sth does not work
//            colNames.push_back(tuple[0]);

            result.insert({pair[0], pair[1]});
        }
        return result;
    }

};
#endif //DBMS_DBUTILS_H
