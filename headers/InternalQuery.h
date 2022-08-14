#ifndef DBMS_INTERNALQUERY_H
#define DBMS_INTERNALQUERY_H

#include "TypeWrapper.h"
#include "Record.h"
#include <string>
#include <unordered_map>

enum class Operation {
    LESS_THAN,
    GREATER_THAN,
    EQUAL,
    NOT_EQUAL,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL,
    NONE
};

//for operations (lhs > rhs)
class InternalQuery {
public:
    InternalQuery() : op(Operation::NONE), isIndexedColumn(false) {}

    InternalQuery(const std::string &lhs, const TypeWrapper &rhs, const std::string &operation,
                  const std::string &indexedColumn) : lhs(lhs), rhs(rhs) {
        setOperation(operation, indexedColumn);
    }

    void setOperation(const std::string &operation, const std::string &indexedColumn) {
        if (operation == "=") {
            op = Operation::EQUAL;
        } else if (operation == ">") {
            op = Operation::GREATER_THAN;
        } else if (operation == "<") {
            op = Operation::LESS_THAN;
        } else if (operation == ">=") {
            op = Operation::GREATER_THAN_OR_EQUAL;
        } else if (operation == "<=") {
            op = Operation::LESS_THAN_OR_EQUAL;
        } else {
            op = Operation::NOT_EQUAL;
        }
//isIndexedColumn = (lhs==indexedColumn)
        if (lhs == indexedColumn) {
            isIndexedColumn = true;
        } else {
            isIndexedColumn = false;
        }
    }

    bool checkRecordAgainstCondition(const std::unordered_map<std::string, int> &indexOfColumns, const Record &rec) const {
        if (indexOfColumns.find(lhs) == indexOfColumns.end()) {
            throw std::invalid_argument("There is no column with name {" + lhs + "} in the table.");
        }

        switch (op) {
            case Operation::GREATER_THAN:
                return rec.getValue(indexOfColumns.at(lhs)) > rhs;
            case Operation::LESS_THAN:
                return rec.getValue(indexOfColumns.at(lhs)) < rhs;
            case Operation::EQUAL:
                return rec.getValue(indexOfColumns.at(lhs)) == rhs;
            case Operation::GREATER_THAN_OR_EQUAL:
                return rec.getValue(indexOfColumns.at(lhs)) > rhs || rec.getValue(indexOfColumns.at(lhs)) == rhs;
            case Operation::LESS_THAN_OR_EQUAL:
                return rec.getValue(indexOfColumns.at(lhs)) < rhs || rec.getValue(indexOfColumns.at(lhs)) == rhs;
            case Operation::NOT_EQUAL:
                return rec.getValue(indexOfColumns.at(lhs)) < rhs || rec.getValue(indexOfColumns.at(lhs)) > rhs;
            default:
                return false;
        }
    }

    bool isPrimaryKeyQuery() const { return isIndexedColumn; }

    TypeWrapper &getValue() { return rhs; }

    Operation getOperator() const { return op; }

private:
    std::string lhs;
    TypeWrapper rhs;
    Operation op;
    bool isIndexedColumn;
};

#endif //DBMS_INTERNALQUERY_H
