#ifndef DBMS_RECORD_H
#define DBMS_RECORD_H

#include "TypeWrapper.h"
#include <vector>
#include <fstream>

class Record {
public:
    Record() : columnsSize(0), isPresent(true) {}

    Record(int columnsSize) : columnsSize(columnsSize), isPresent(true) {
        this->values.reserve(columnsSize);
    }

    Record(std::ifstream &in) {
        in.read((char*)&isPresent, sizeof(isPresent));
        in.read((char*)&columnsSize, sizeof(columnsSize));
        for (size_t i = 0; i < columnsSize; i++) {
            values.push_back(TypeWrapper(in));
        }
    }

    void write(std::ofstream &out) const {
        out.write((char*)&isPresent, sizeof(isPresent));
        out.write((char*)&columnsSize, sizeof(columnsSize));
        for (size_t i = 0; i < values.size(); i++) {
            values[i].write(out);
        }
    }

    void insert(const TypeWrapper& typeWrapper) {
        if(values.size() + 1 > columnsSize) {
            throw std::out_of_range("Maximum columns for this record reached");
        }
        values.push_back(typeWrapper);
    }

    void removeRecord() {
        isPresent = false;
        values.clear();
    }

    bool isPresentRecord() {
        return isPresent;
    }

    const TypeWrapper& getValue(int indexToGet) const {
        if (indexToGet >= values.size()) {
            throw std::out_of_range("Record getValue(index) - index is out of range");
        }
        return values[indexToGet];
    }

    int getSizeInBytes() {
        int bytes = 0;
        for(const TypeWrapper& value: values) {
            bytes += value.getContent()->sizeInBytes();
        }
        return bytes;
    }

    bool isEqualToAccordingColumns(const Record& otherRecord, std::vector<int>& columns) {
        if(values.size() != otherRecord.values.size()) {
            return false;
        }
        for (int i = 0; i < columns.size(); ++i) {
            int curr = columns[i];
            if(values[curr] != otherRecord.values[curr]) {
                return false;
            }
        }
        return true;
    }

    bool operator==(const Record& other) const {
        if (other.columnsSize != columnsSize) {
            return false;
        }
        for (size_t i = 0; i < columnsSize; i++) {
            if (values[i] != other.values[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator<(const Record& other) const {
        if (other.columnsSize != columnsSize) {
            return false;
        }
        for (size_t i = 0; i < columnsSize; i++) {
            if (values[i] >= other.values[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator>(const Record& other) const {
        if (other.columnsSize != columnsSize) {
            return false;
        }
        for (size_t i = 0; i < columnsSize; i++) {
            if (values[i] <= other.values[i]) {
                return false;
            }
        }
        return true;
    }
private:
    bool isPresent;
    int columnsSize;
    std::vector<TypeWrapper> values;
};


#endif //DBMS_RECORD_H
