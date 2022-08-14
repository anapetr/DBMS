#ifndef DBMS_STRINGTYPE_H
#define DBMS_STRINGTYPE_H

#include <string>
#include <fstream>

#include "Object.h"
#include "ObjectType.h"
#include "../utils/FileUtils.h"

class StringType : public Object {
public:
    StringType() {}

    StringType(std::string value) : value(value) {}

    virtual Object *clone() const final override {
        return new StringType(*this);
    }

    virtual size_t sizeInBytes() const final override {
        return value.size();
    }

    virtual std::string toString() const final override {
        return value;
    }

    virtual size_t size() const final override {
        return value.size();
    }

    virtual void write(std::ofstream &out) const final override {
        ObjectType s = ObjectType::STRING;
        out.write((char *) &s, sizeof(s));
        FileUtils::writeString(out, value);
    }

private:
    std::string value;

    virtual bool isGreaterThan(const Object &other) const override {
        return value > static_cast<const StringType &>(other).value;
    }

    virtual bool isEqualTo(const Object &other) const override {
        return value == static_cast<const StringType &>(other).value;
    }

    virtual bool isLessThan(const Object &other) const override {
        return value < static_cast<const StringType &>(other).value;
    }
};


#endif //DBMS_STRINGTYPE_H
