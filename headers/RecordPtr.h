#ifndef DBMS_RECORDPTR_H
#define DBMS_RECORDPTR_H

#include <fstream>

class RecordPtr { //children to record in a file

public:
    RecordPtr(std::ifstream &in) {
        in.read((char *) &numOfPage, sizeof(numOfPage));
        in.read((char *) &recordIndexInPage, sizeof(recordIndexInPage));
    }

    RecordPtr(int numOfPage, int recordIndexInPage) : numOfPage(numOfPage), recordIndexInPage(recordIndexInPage) {}

    int getPageNumber() const {
        return numOfPage;
    }

    int getRecordIndexInPage() const {
        return recordIndexInPage;
    }

    void write(std::ofstream &out) const {
        out.write((char *) &numOfPage, sizeof(numOfPage));
        out.write((char *) &recordIndexInPage, sizeof(recordIndexInPage));
    }

    bool operator<(const RecordPtr &other) const {
        if (numOfPage < other.numOfPage)
            return true;

        return numOfPage == other.numOfPage && recordIndexInPage < other.recordIndexInPage;
    }

    bool operator>(const RecordPtr &other) const {
        if (numOfPage > other.numOfPage)
            return true;

        return numOfPage == other.numOfPage && recordIndexInPage > other.recordIndexInPage;
//
//        else if (numOfPage == other.numOfPage)
//            if (recordIndexInPage > other.recordIndexInPage)
//                return true;
//
//        return false;
    }

    bool operator==(const RecordPtr &other) const {
        return numOfPage == other.numOfPage && recordIndexInPage == other.recordIndexInPage;
    }

private:
    int numOfPage{-1};
    int recordIndexInPage{-1};
};

#endif //DBMS_RECORDPTR_H
