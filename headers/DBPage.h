#ifndef DBMS_DBPAGE_H
#define DBMS_DBPAGE_H

#include <vector>
#include "Record.h"
#include "../utils/FileUtils.h"

class DBPage {
public:
    DBPage(int maxCapacity, const std::string &pathToDBPage) : maxCapacity(maxCapacity), pathToDBPage(pathToDBPage) {
        saveContentToFile();
    }

    DBPage(std::ifstream &in) {
        in.read((char *) &maxCapacity, sizeof(maxCapacity));

        FileUtils::readString(in, pathToDBPage);

        int currentSize = 0;
        in.read((char *) &currentSize, sizeof(currentSize));
        for (size_t i = 0; i < currentSize; i++) {
            content.push_back(Record(in));
        }
    }

    int getContentSize() {
        return content.size();
    }

    bool insert(const Record &record) {
        if (content.size() == maxCapacity) {
            return false;
        }
        content.push_back(record);
        saveContentToFile();

        return true;
    }

    // we will not delete the record, but only we will set is as invalid
    void remove(int indexToRemove) {
        if (indexToRemove >= 0 && indexToRemove < content.size()) {
            content[indexToRemove].removeRecord();
            saveContentToFile();
        } else {
            throw std::out_of_range(std::to_string(indexToRemove) + " is out of range");
        }
    }

    Record getRecord(int indexToGet) {
        if (indexToGet >= 0 && indexToGet < content.size()) {
            return content[indexToGet];
        }
        throw std::out_of_range(std::to_string(indexToGet) + " is out of range");
    }

    void saveContentToFile() {
        std::ofstream out(pathToDBPage, std::ios::binary);
        if (!out.is_open()) {
            throw std::logic_error("Couldn't open file to save page " + pathToDBPage);
        }

        out.write((char *) &maxCapacity, sizeof(maxCapacity));
        FileUtils::writeString(out, pathToDBPage);

        int currentSize = content.size();
        out.write((char *) &currentSize, sizeof(currentSize));

        for (int i = 0; i < currentSize; i++) {
            content[i].write(out);
        }
        out.close();
    }

    bool isFull() {
        return content.size() == maxCapacity;
    }

private:
    int maxCapacity;
    std::string pathToDBPage;
    std::vector<Record> content;
};

#endif //DBMS_DBPAGE_H