#ifndef DBMS_INTEGERTYPE_H
#define DBMS_INTEGERTYPE_H

#include <string>
#include <fstream>

#include "Object.h"
#include "ObjectType.h"
#include "../utils/FileUtils.h"

class IntegerType : public Object {
public:
    IntegerType() : value(0) {}

    IntegerType(int value) : value(value) {}

    virtual Object* clone() const final override {
        return new IntegerType(*this);
    }

    virtual size_t sizeInBytes() const final override {
        return sizeof(value);
    }

    virtual std::string toString() const final override {
        return std::to_string(value);
    }

    virtual size_t size() const final override {
        return std::to_string(value).size();
    }

    virtual void write(std::ofstream& out) const final override {
        ObjectType i = ObjectType::INT;
        out.write((char*)&i, sizeof(i));
        out.write((char*)&value, sizeof(value));
    }

private:
    int value;

    virtual bool isGreaterThan(const Object& other) const final override {
        return value > static_cast<const IntegerType&>(other).value;
    }

    virtual bool isEqualTo(const Object& other) const final override {
        return value == static_cast<const IntegerType&>(other).value;
    }

    virtual bool isLessThan(const Object& other) const final override {
        return value < static_cast<const IntegerType&>(other).value;
    }
};


#endif //DBMS_INTEGERTYPE_H
