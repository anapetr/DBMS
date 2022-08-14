#ifndef DBMS_DBQUERY_H
#define DBMS_DBQUERY_H

#include "../utils/StringUtils.h"
#include "Record.h"
#include "InternalQuery.h"

#include <string>
#include <unordered_map>
#include <queue>

class DBQuery {
public:
    DBQuery(std::string expression, const std::unordered_map<std::string, std::string>& nameTypeOfColumns, const std::string& nameOfIndexedColumn);

    std::queue<std::string> shuntingYardAlgorithm(std::string expression);

    int priority(const std::string& op) const;

    bool assertRecordAgainstQuery(const Record& r, const std::unordered_map<std::string, int>& indexOfColumns) const;

    std::string parseExpression(std::string expression, const std::unordered_map<std::string, std::string>& nameTypeOfColumns, const std::string& nameOfIndexedColumn);

    TypeWrapper decideType(const std::string& value) const;

    std::queue<std::string> getQueueQuery();

    std::unordered_map<std::string, InternalQuery> getNumberedQueries();

private:
    std::string inputQuery;
    std::queue<std::string> queueQuery;
    std::unordered_map<std::string, InternalQuery> numberedQueries;
    std::vector<InternalQuery> indexedColumnQueries;
};


#endif //DBMS_DBQUERY_H
