#ifndef DBMS_NODEKEY_H
#define DBMS_NODEKEY_H


#include "TypeWrapper.h"
#include "RecordPtr.h"

/**
 * @brief This is the data that a node of the tree contains
 * typeWrapper is the actual data
 * recordPtr is where to find it in the files
 */
struct NodeKey {
    TypeWrapper typeWrapper;
    RecordPtr recordPtr;

    NodeKey(const TypeWrapper &typeWrapper, const RecordPtr &recordPtr) : typeWrapper(typeWrapper),
                                                                          recordPtr(recordPtr) {}
};


#endif //DBMS_NODEKEY_H
