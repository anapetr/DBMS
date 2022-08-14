#include "../headers/DBTable.h"

DBTable::DBTable() : maxRecordsInPage(1024), currentPage(0), columnsCount(0), sizeOfTableInBytes(0) {}

DBTable::DBTable(const std::string &folderPath,
                 const std::string &tableName,
                 const std::string &nameOfIndexedColumn,
                 int maxRecordsPerPage,
                 std::vector<std::string> columns,
                 std::unordered_map<std::string, std::string> &nameTypeOfColumns) {

    this->folderPath = folderPath;
    this->tableName = tableName;
    this->nameOfIndexedColumn = nameOfIndexedColumn;
    this->nameTypeOfColumns = nameTypeOfColumns;
    this->maxRecordsInPage = maxRecordsPerPage;
    this->currentPage = -1;
    this->columnsCount = 0;
    this->sizeOfTableInBytes = 0;

    //create directory for every DB in order to keep records cleaner and create the first page
    FileUtils::createFolder(folderPath);
    addNewPage();

    //create the B+ Tree
    if (!this->nameOfIndexedColumn.empty() && this->nameTypeOfColumns.find(this->nameOfIndexedColumn) != this->nameTypeOfColumns.end()) {
        indexTable(nameOfIndexedColumn);
    }

    parseInfoForColumns(columns);
    indexColumns(columns);
    saveTableToFile();
}

DBTable::DBTable(std::ifstream &in) {
    in.read((char *) &currentPage, sizeof(currentPage));
    in.read((char *) &maxRecordsInPage, sizeof(maxRecordsInPage));
    in.read((char *) &sizeOfTableInBytes, sizeof(sizeOfTableInBytes));

    FileUtils::readString(in, folderPath);
    FileUtils::readString(in, tableName);
    FileUtils::readString(in, nameOfIndexedColumn);
    FileUtils::readString(in, infoForColumns);

    int colCount = 0;
    in.read((char *) &colCount, sizeof(colCount));
    for (int i = 0; i < colCount; i++) {
        std::string nameOfColumn;
        std::string typeOfColumn;
        FileUtils::readString(in, nameOfColumn);
        FileUtils::readString(in, typeOfColumn);
        nameTypeOfColumns.insert({nameOfColumn, typeOfColumn});
    }

    if (!nameOfIndexedColumn.empty()) {
        bPlusTree = BPlusTree(in);
    }

    std::vector<std::string> header = StringUtils::splitLineByDelimiter(infoForColumns, ",");
    StringUtils::removeEmptyStringsInVector(header);
    indexColumns(header);
}

std::string DBTable::getTablePath() const { //in order to access the right pages
    return folderPath + tableName + ".bin";
}

void DBTable::tableInfo() const {
    std::string header = getInfoForColumns();
    StringUtils::trim(header);

    std::vector<std::string> cols = StringUtils::splitLineByDelimiter(header, ",");
    StringUtils::removeEmptyStringsInVector(cols);

    std::string scheme = "(";
    for (const std::string& name : cols) {
        scheme += name + ":" + nameTypeOfColumns.at(name) + ", ";
    }

    scheme += ") " + (nameOfIndexedColumn.empty() ? "No Index on this table" : ("Index ON " + nameOfIndexedColumn));

    std::cout << "Table " << tableName << " : " << scheme << std::endl;

    if (sizeOfTableInBytes < 1024) {
        std::cout << "(" << sizeOfTableInBytes << " B data) in the table" << std::endl;
    } else {
        std::cout << "(" << (sizeOfTableInBytes / 1024) << " KB data) in the table" << std::endl;
    }
}


void DBTable::saveTableToFile() {
    std::ofstream out(folderPath + tableName + ".bin", std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Couldn't open file to save the table");
    }

    out.write((char *) &currentPage, sizeof(currentPage));
    out.write((char *) &maxRecordsInPage, sizeof(maxRecordsInPage));
    out.write((char *) &sizeOfTableInBytes, sizeof(sizeOfTableInBytes));

    FileUtils::writeString(out, folderPath);
    FileUtils::writeString(out, tableName);
    FileUtils::writeString(out, nameOfIndexedColumn);
    FileUtils::writeString(out, infoForColumns);

    int colTypesSize = nameTypeOfColumns.size();
    out.write((char *) &colTypesSize, sizeof(colTypesSize));

    for (std::pair<std::string, std::string> column: nameTypeOfColumns) {
        FileUtils::writeString(out, column.first);
        FileUtils::writeString(out, column.second);
    }

    if (!nameOfIndexedColumn.empty()) {
        bPlusTree.saveTreeInFile(out);
    }
    out.close();
}

void DBTable::indexColumns(std::vector<std::string> columns) {
    int i = 0;
    for (const std::string &column: columns) {
        this->indexOfColumns.insert({column, i++});
    }
    this->columnsCount = columns.size();
}

//according to primary key, to create pointer between record and tree
void DBTable::indexTable(std::string nameOfIndexedColumn) {
    //we go through every page in the folder of this DBTable
    for (int i = 0; i <= currentPage; i++) {
        std::ifstream in(folderPath + tableName + "_page_" + std::to_string(i) + ".bin", std::ios::binary);

        DBPage page(in);
        //обхождаме всеки ред от тaблицата и на колона с позиция pos взимаме стойността и я insert-ваме в дървото и рпавим и recordPtr
        for (int j = 0; j < page.getContentSize(); ++j) {
            Record record = page.getRecord(j);

            if (record.isPresentRecord()) {
                RecordPtr recordPtr(i, j);
                int positionOfColumn = indexOfColumns.at(nameOfIndexedColumn);
                bPlusTree.insert(NodeKey(record.getValue(positionOfColumn), recordPtr));
            }
        }
        in.close();
    }
    saveTableToFile();
}

void DBTable::checkNewRecordsAgainstTypes(std::unordered_map<std::string, TypeWrapper> &newRecord) {
    for (const std::pair<std::string, TypeWrapper> &value: newRecord) {
        std::string nameOfColumn = value.first;
        if (nameTypeOfColumns.find(nameOfColumn) == nameTypeOfColumns.end()) {
            throw std::invalid_argument("Column " + nameOfColumn + " does not exist in the table");
        }

        TypeWrapper valueOfColumn = value.second;
        std::string typeOfColumn = nameTypeOfColumns.at(nameOfColumn);

        //checking if the type is correct and if the value is representing the type
        bool isValidType = true;
        if (typeOfColumn == "Integer" && typeid(*valueOfColumn.getContent()) != typeid(IntegerType)) {
            isValidType = false;
        }

        if (typeOfColumn == "String" && typeid(*valueOfColumn.getContent()) != typeid(StringType)) {
            isValidType = false;
        }

//        if (typeOfColumn == "DATE" && typeid(*value.valueOfColumn()) != typeid(DateType)) {
//            isValidType = false;
//        }

        if (!isValidType) {
            throw std::invalid_argument(
                    "Type of column " + nameOfColumn + " does not match with the type in the table");
        }
    }
}

DBPage DBTable::addNewPage() {
    std::string pathToPage = folderPath + tableName + "_page_" + std::to_string(++currentPage) + ".bin";
    DBPage newPage(maxRecordsInPage, pathToPage);
    saveTableToFile();
    return newPage;
}

std::vector<int> DBTable::getColumnsIndexesByName(const std::vector<std::string> &columnsNames) {
    std::vector<int> result;

    for (int i = 0; i < columnsNames.size(); ++i) {
        result.push_back(indexOfColumns.at(columnsNames[i]));
    }

    return result;
}

void DBTable::insert(std::unordered_map<std::string, TypeWrapper> &newRecord) {
    checkNewRecordsAgainstTypes(newRecord);

    if (!nameOfIndexedColumn.empty()) {
        //if the column exists
        if (newRecord.at(nameOfIndexedColumn).getContent() == nullptr) {
            throw std::invalid_argument("Indexed column " + nameOfIndexedColumn + " cannot be empty");
        }

        // do we have other record with the same key value
        if (checkRecordExists(nameOfIndexedColumn, newRecord.at(nameOfIndexedColumn))) {
            throw std::invalid_argument("Indexed column's values must be unique");
        }
    }

    //adds the values in the record(vector)
    Record recordToAdd(columnsCount);
    std::vector<std::string> orderedColumnNames = getOrderedColumnNames();
    for (const std::string& entry : orderedColumnNames) {
        recordToAdd.insert(newRecord[entry]);
    }

    //add to page
    std::string pathToCurrPage = folderPath + tableName + "_page_" + std::to_string(currentPage) + ".bin";
    std::ifstream in(pathToCurrPage, std::ios::binary);

    if (!in.is_open()) {
        throw std::invalid_argument("Could not open page at path " + pathToCurrPage + " for reading.");
    }

    DBPage page(in);
    if (page.isFull()) {
        page = addNewPage();
    }

    page.insert(recordToAdd);
    sizeOfTableInBytes += recordToAdd.getSizeInBytes();

    in.close();

    //add in B + tree

    if (!nameOfIndexedColumn.empty()) {
        bPlusTree.insert({newRecord.at(nameOfIndexedColumn), RecordPtr(currentPage, page.getContentSize() - 1)});
    }

    saveTableToFile();
}

bool DBTable::checkRecordExists(const std::string& colName, const TypeWrapper& colValue) {
    DBQuery expression(colName + " = " + colValue.asString(), nameTypeOfColumns, nameOfIndexedColumn);
    std::vector<Record> answer = select(expression);

    return !answer.empty();
}

std::vector<Record> DBTable::select(DBQuery &query) {
    std::stack<std::vector<Record> > result;
    std::queue<std::string> queueQuery = query.getQueueQuery();

    while (!queueQuery.empty()) {
        std::string currElement = queueQuery.front();
        if (StringUtils::isInteger(currElement)) {
            InternalQuery curr = query.getNumberedQueries().at(currElement);
            //get from tree if we have primary key
            if (curr.isPrimaryKeyQuery()) {
                std::vector<RecordPtr> fromTree = bPlusTree.getRecordsFromQuery(curr);
                if (!fromTree.empty()) {
                    result.push(getRecords(fromTree));
                } else {
                    result.push(std::vector<Record>());
                }
            } else {
                std::vector<Record> answer;
                for (size_t index = 0; index <= currentPage; index++) {
                    std::ifstream in(folderPath + tableName + "_page_" + std::to_string(index) + ".bin",
                                     std::ios::binary);

                    DBPage page(in);
                    for (size_t i = 0; i < page.getContentSize(); ++i) {
                        Record currRecord = page.getRecord(i);
                        if (currRecord.isPresentRecord()) {
                            if (curr.checkRecordAgainstCondition(indexOfColumns, currRecord)) {
                                answer.push_back(currRecord);
                            }
                        }
                    }
                    in.close();
                }
                result.push(answer);
            }
            queueQuery.pop();
        } else {
            std::vector<Record> val2 = result.top();
            result.pop();

            std::vector<Record> val1 = result.top();
            result.pop();

            std::string operation = queueQuery.front();
            queueQuery.pop();

            if (operation == "AND") {
                std::vector<Record> intersection;
                if (val2.size() > val1.size()) {
                    std::swap(val1, val2);
                }
                for (int i = 0; i < val2.size(); i++) {
                    if (std::find(val1.begin(), val1.end(), val2[i]) != val1.end()) {
                        intersection.push_back(val2[i]);
                    }
                }
                result.push(intersection);
            } else {
                std::vector<Record> recordsUnion;
                for (Record &currRecord: val1) {
                    if (std::find(recordsUnion.begin(), recordsUnion.end(), currRecord) == recordsUnion.end()) {
                        recordsUnion.push_back(currRecord);
                    }
                }
                for (Record &currRecord: val2) {
                    if (std::find(recordsUnion.begin(), recordsUnion.end(), currRecord) == recordsUnion.end()) {
                        recordsUnion.push_back(currRecord);
                    }
                }
                result.push(recordsUnion);
            }
        }
    }
    return result.top();
}

std::vector<Record> DBTable::select(DBQuery& query, const std::string& orderByWhat, bool isDistinct, std::vector<std::string>& selectedColumns){
    std::vector<Record> answer;
    if (!query.getQueueQuery().empty()) {
        answer = select(query);
    } else {
        for (size_t index = 0; index <= currentPage; index++) {
            std::ifstream in(folderPath + tableName + "_page_" + std::to_string(index) + ".bin", std::ios::binary);

            DBPage page(in);
            for (size_t i = 0; i < page.getContentSize(); ++i) {
                Record record = page.getRecord(i);
                if (record.isPresentRecord())
                    answer.push_back(record);
            }
            in.close();
        }
    }

    if (isDistinct) {
        answer = distinct(answer, selectedColumns);
    }
    if (!orderByWhat.empty()) {
        orderBy(answer, orderByWhat);
    }
    return answer;
}

int DBTable::remove(DBQuery &query) {
    int deletedRecords = 0;
    if (!query.getQueueQuery().empty()) {
        if (!nameOfIndexedColumn.empty()) {
            //take all records against query and remove them
            std::vector<Record> recordsToRemove = select(query);

            for (int i = 0; i < recordsToRemove.size(); ++i) {
                //remove from page(invalidate), B+ tree and calculate new bytes
                Record currRecord = recordsToRemove[i];
                if (!currRecord.isPresentRecord()) {
                    continue;
                }
                RecordPtr recordPtr = bPlusTree.getRecordAtIndex(
                        currRecord.getValue(indexOfColumns[nameOfIndexedColumn]));

                std::ifstream in(folderPath + tableName + "_page_" + std::to_string(recordPtr.getPageNumber()) + ".bin",
                                 std::ios::binary);
                DBPage page(in);
                page.remove(recordPtr.getRecordIndexInPage());

                sizeOfTableInBytes -= currRecord.getSizeInBytes();
                bPlusTree.remove(currRecord.getValue(indexOfColumns[nameOfIndexedColumn]));

                deletedRecords++;

                in.close();
            }
        } else {
            //we open every page and invalidating the records
            for (int i = 0; i <= currentPage; i++) {
                std::ifstream in(folderPath + tableName + "_page_" + std::to_string(i) + ".bin", std::ios::binary);
                DBPage page(in);
                for (int j = 0; j < page.getContentSize(); i++) {
                    Record currRecord = page.getRecord(j);
                    if (currRecord.isPresentRecord() && query.assertRecordAgainstQuery(currRecord, indexOfColumns)) {
                        sizeOfTableInBytes -= currRecord.getSizeInBytes();
                        page.remove(j);
                        deletedRecords++;
                    }
                }
            }
        }
    }
    saveTableToFile();
    return deletedRecords;
}

std::vector<Record> DBTable::distinct(std::vector<Record> &recordsToDistinct, std::vector<std::string> &columns) {
    std::vector<int> indexes = getColumnsIndexesByName(columns);
    for (const std::string &column: columns) {
        if (indexOfColumns.find(column) == indexOfColumns.end()) {
            throw std::invalid_argument("Cannot call distinct on non existing column with name " + column);
        }
    }

    std::vector<Record> result;
    for (int i = 0; i < recordsToDistinct.size(); ++i) {
        bool areEqual = false;
        for (int j = i + 1; j < recordsToDistinct.size(); ++j) {
            if (recordsToDistinct[i].isEqualToAccordingColumns(recordsToDistinct[j], indexes)) {
                areEqual = true;
                break;
            }
        }
        if (!areEqual) {
            result.push_back(recordsToDistinct[i]);
        }
    }
    return result;
}

void DBTable::orderBy(std::vector<Record> &recordsToOrder, const std::string &orderByWhat) {
    if (indexOfColumns.find(orderByWhat) == indexOfColumns.end()) {
        throw std::invalid_argument("Cannot call distinct on non existing column with name " + orderByWhat);
    }
    DBUtils::heapSort(recordsToOrder, indexOfColumns[orderByWhat]);
}

//getRecord records from RecordPointers from BPlus tree
std::vector<Record> DBTable::getRecords(std::vector<RecordPtr> &recordPtrs) {
    std::vector<Record> result;

    // Sort pointers by page in ascending order, then by index in page in ascending order
    for (int i = 0; i < recordPtrs.size() - 1; i++) {
        int min = i;
        for (size_t j = i + 1; j < recordPtrs.size(); j++) {
            if (recordPtrs[j] < recordPtrs[min]) {
                min = j;
            }
        }
        std::swap(recordPtrs[min], recordPtrs[i]);
    }

    // Whisk through the pages, first opening the one with index 0, then 1, then 2...
    // and extracting the needed records. Prioritizes extracting the records from the current page, then the next, without
    // having to reopen on every iteration for every record
    for (int i = 0; i < recordPtrs.size(); i++) {
        std::ifstream in(folderPath + tableName + "_page_" + std::to_string(recordPtrs[i].getPageNumber()) + ".bin",
                         std::ios::binary);
        DBPage page(in);
        Record currRecord = page.getRecord(recordPtrs[i].getRecordIndexInPage());

        if (i < recordPtrs.size() - 1 && recordPtrs[i].getPageNumber() != recordPtrs[i + 1].getPageNumber()) {
            if (page.getRecord(recordPtrs[i].getRecordIndexInPage()).isPresentRecord()) {
                result.push_back(page.getRecord(recordPtrs[i].getRecordIndexInPage()));
            }
        } else {
            while (i < recordPtrs.size() - 1 && recordPtrs[i].getPageNumber() == recordPtrs[i + 1].getPageNumber()) {
                if (page.getRecord(recordPtrs[i].getRecordIndexInPage()).isPresentRecord()) {
                    result.push_back(page.getRecord(recordPtrs[i].getRecordIndexInPage()));
                }
                i++;
            }
        }
        if (i == recordPtrs.size() - 1) {
            if (page.getRecord(recordPtrs[i].getRecordIndexInPage()).isPresentRecord()) {
                result.push_back(page.getRecord(recordPtrs[i].getRecordIndexInPage()));
            }
        }
        in.close();
    }
    return result;
}

void DBTable::parseInfoForColumns(std::vector<std::string>& columns) {
    for (const std::string& name : columns) {
        infoForColumns += name + ",";
    }
}

std::vector<std::string> DBTable::getOrderedColumnNames() const {
    std::vector<std::string> result = StringUtils::splitLineByDelimiter(infoForColumns, ",");
    StringUtils::removeEmptyStringsInVector(result);
    return result;
}

std::unordered_map<std::string, std::string> DBTable::getNameTypeOfColumns() const {
    return nameTypeOfColumns;
}

std::string DBTable::getNameOfIndexedColumn() const {
    return nameOfIndexedColumn;
}

std::unordered_map<std::string, int> DBTable::getIndexOfColumns() const {
    return indexOfColumns;
}

std::string DBTable::getInfoForColumns() const {
    return infoForColumns;
}