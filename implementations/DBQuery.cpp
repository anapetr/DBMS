#include "../headers/DBQuery.h"

DBQuery::DBQuery(std::string expression, const std::unordered_map<std::string, std::string> &nameTypeOfColumns,
                 const std::string &nameOfIndexedColumn) {
    inputQuery = parseExpression(expression, nameTypeOfColumns, nameOfIndexedColumn);
    queueQuery = shuntingYardAlgorithm(inputQuery);
}

std::queue<std::string> DBQuery::shuntingYardAlgorithm(std::string expression) {
    std::queue<std::string> result;
    std::stack<std::string> operations;

    while (!expression.empty()) {
        int position = expression.find(" ");

        std::string element;
        if (position != expression.npos) {
            element = expression.substr(0, position);
        } else {
            element = expression;
        }

        if (element == " ") {
            continue;
        } else if (element == "(") {
            operations.push(element);
        } else if ((StringUtils::isInteger(element))) {
            result.push(element);
        } else if (element == ")") {
            while (/*!operations.empty() && */operations.top() != "(") {
                result.push(operations.top());
                operations.pop();
            }
        } else if (element == "OR" || element == "AND") {
            if(!operations.empty()) {
                while (priority(operations.top()) >= priority(element)) {
                    result.push(operations.top());
                    operations.pop();
                }
            }
            operations.push(element);
        }
        if(position != expression.npos) {
            expression.erase(0, position + 1);
        } else {
            expression.erase(0, expression.size());
        }
    }
    while (!operations.empty()) {
        result.push(operations.top());
        operations.pop();
    }
    return result;
}

int DBQuery::priority(const std::string &op) const {
    if (op == "OR")
        return 1;
    if (op == "AND")
        return 2;
    if (op == "NOT")
        return 3;

    return 0;
}

//we have the postfix result from shunting yard algorithm, so we are going to check if the record satisfies the query
bool DBQuery::assertRecordAgainstQuery(const Record &record,
                                       const std::unordered_map<std::string, int> &indexOfColumns) const {
    std::queue<std::string> query = queueQuery;

    std::stack<bool> result;
    std::vector<Record> res;

    while (!queueQuery.empty()) {
        if (StringUtils::isInteger(query.front())) {
            result.push(numberedQueries.at(query.front()).checkRecordAgainstCondition(indexOfColumns, record));
            query.pop();
        } else {
            bool val2 = result.top();
            result.pop();

            bool val1 = result.top();
            result.pop();

            std::string op = query.front();
            query.pop();

            if (op == "AND") {
                result.push(val1 && val2);
            } else {
                result.push(val1 || val2);
            }
        }
    }
    return result.top();
}

//we prepare the expression for shunting yard
std::string
DBQuery::parseExpression(std::string expression, const std::unordered_map<std::string, std::string> &nameTypeOfColumns,
                         const std::string &nameOfIndexedColumn) {
    int index = 0;
    int position;
    std::string result;

    while ((position = expression.find(" ")) != expression.npos) {
        std::string subStr = expression.substr(0, position);
        //if it is column from table - take column, operation and value and build Internal Query
        if (nameTypeOfColumns.find(subStr) != nameTypeOfColumns.end()) {
            std::string column;
            std::string operation;
            std::string value;
            column = subStr;// Column name
            expression.erase(0, position + 1);

            position = expression.find(" ");
            subStr = expression.substr(0, position); // operator 
            operation = subStr;
            expression.erase(0, position + 1);

            position = expression.find(" ");
            subStr = expression.substr(0, position);
            //if value is string
            if (subStr.find("\"") != expression.npos) {
                int numQuotes = 0;
                int i;
                for (i = 0; i < expression.size(); i++) {
                    if (expression[i] == '"') {
                        numQuotes++;
                    }
                    //we take it with quotes
                    value += expression[i];
                    if (numQuotes % 2 == 0) {
                        break;
                    }
                }
                position = i;
            } else {
                subStr = expression.substr(0, position); // Column value
                value = subStr;
            }

            result += " " + std::to_string(index);

            // Check if query contains primary key, if so then add it to the array of primary key queries
            InternalQuery query(column, decideType(value), operation, nameOfIndexedColumn);
            if (column == nameOfIndexedColumn) {
                indexedColumnQueries.push_back(query);
            }
            numberedQueries[std::to_string(index++)] = query;
        }
            //we add it to result in order to have ( 0 AND ( 1 OR 2 ) )
        else if (subStr == "AND" || subStr == "OR" || subStr == "NOT") {
            result += " " + subStr;
        } else if (subStr == "(" || subStr == ")") {
            result += " " + subStr;
        }

        if (position != expression.npos) {
            expression.erase(0, position + 1);
        } else {
            expression.erase(0, expression.size());
        }
    }
    if (!expression.empty()) {
        result += " " + expression;
    }
    return StringUtils::trim(result);
}

TypeWrapper DBQuery::decideType(const std::string &value) const {
    if (StringUtils::isInteger(value)) {
        return TypeWrapper(stoi(value));
    } else if (StringUtils::isValidString(value)) {
        return TypeWrapper(value);
    }
}

std::queue<std::string> DBQuery::getQueueQuery() {
    return queueQuery;
}

//we have index compared to InternalQuery
std::unordered_map<std::string, InternalQuery> DBQuery::getNumberedQueries() {
    return numberedQueries;
}