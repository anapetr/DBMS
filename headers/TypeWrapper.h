#ifndef DBMS_TYPEWRAPPER_H
#define DBMS_TYPEWRAPPER_H

#include <fstream>
#include "Object.h"
#include "ObjectType.h"
#include "StringType.h"
#include "IntegerType.h"

class TypeWrapper {
public:
    TypeWrapper();

    TypeWrapper(const TypeWrapper &other);

    TypeWrapper &operator=(const TypeWrapper &other);

    ~TypeWrapper();

    TypeWrapper(const std::string &content);

    TypeWrapper(int content);

    //TypeWrapper(int date); use miliseconds

    Object* getContent() const;

    TypeWrapper(std::ifstream &in);

    void write(std::ofstream& out) const;

    std::string asString() const;

    bool operator>(const TypeWrapper &other) const;

    bool operator==(const TypeWrapper &other) const;

    bool operator<(const TypeWrapper &other) const;

    bool operator<=(const TypeWrapper &other) const;

    bool operator>=(const TypeWrapper &other) const;

    bool operator!=(const TypeWrapper &other) const;

private:
    Object *content;

    void copyFrom(const TypeWrapper &other);
};


#endif //DBMS_TYPEWRAPPER_H
