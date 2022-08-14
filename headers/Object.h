#ifndef DBMS_OBJECT_H
#define DBMS_OBJECT_H

#include <fstream>
#include <string>

class Object {
public:
    virtual Object* clone() const = 0;
    virtual size_t sizeInBytes() const = 0; //memory size
    virtual std::string toString() const = 0;
    virtual void write(std::ofstream& out) const = 0;
    virtual size_t size() const = 0;

    bool operator>(const Object& other) const
    {
        return typeid(*this) == typeid(other) && isGreaterThan(other);
    }

    bool operator==(const Object& other) const
    {
        return typeid(*this) == typeid(other) && isEqualTo(other);
    }

    bool operator<(const Object& other) const
    {
        return typeid(*this) == typeid(other) && isLessThan(other);
    }

private:
    virtual bool isGreaterThan(const Object& other) const = 0;

    virtual bool isEqualTo(const Object& other) const = 0;

    virtual bool isLessThan(const Object& other) const = 0;
};


#endif //DBMS_OBJECT_H
