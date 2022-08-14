#include "../headers/TypeWrapper.h"

TypeWrapper::TypeWrapper() : content(nullptr) {}

TypeWrapper::TypeWrapper(const TypeWrapper& other) {
    copyFrom(other);
}

TypeWrapper& TypeWrapper::operator=(const TypeWrapper& other){
    if (this != &other) {
        delete content;
        copyFrom(other);
    }
    return *this;
}

void TypeWrapper::copyFrom(const TypeWrapper& other) {
    if (other.content != nullptr) {
        content = other.content->clone();
    } else {
        content = nullptr;
    }
}

Object* TypeWrapper::getContent() const {
    return content;
}

TypeWrapper::~TypeWrapper() {
    delete content;
}

TypeWrapper::TypeWrapper(const std::string &content) : content(new StringType(content)) {}

TypeWrapper::TypeWrapper(int content) : content(new IntegerType(content)) {}

//TypeWrapper(int date);

TypeWrapper::TypeWrapper(std::ifstream &in) {
    ObjectType objectType = ObjectType::INT;
    in.read((char *) &objectType, sizeof(objectType));

    if (objectType == ObjectType::INT) {
        int value = 0;
        in.read((char *) &value, sizeof(value));
        content = new IntegerType(value);
    } else if (objectType == ObjectType::STRING) {
        std::string value;
        FileUtils::readString(in, value);
        content = new StringType(value);
    }
//    else if (objectType == ObjectType::DATE)
//    {
//        in.read((char*)&value, sizeof(value));
//        fContent = new DoubleObject(value);
//    }
    else {
        content = nullptr;
    }
}

void TypeWrapper::write(std::ofstream& out) const {
    content->write(out);
}

bool TypeWrapper::operator>(const TypeWrapper &other) const {
    return content->operator>(*other.content);
}

bool TypeWrapper::operator==(const TypeWrapper &other) const {
    return content->operator==(*other.content);
}

bool TypeWrapper::operator<(const TypeWrapper &other) const {
    return content->operator<(*other.content);
}

bool TypeWrapper::operator<=(const TypeWrapper &other) const {
    return content->operator<(*other.content) || content->operator==(*other.content);
}

bool TypeWrapper::operator>=(const TypeWrapper &other) const {
    return content->operator<(*other.content) || content->operator==(*other.content);
}

bool TypeWrapper::operator!=(const TypeWrapper &other) const {
    return content->operator<(*other.content) || content->operator>(*other.content);
}

std::string TypeWrapper::asString() const {
    return content->toString();
}