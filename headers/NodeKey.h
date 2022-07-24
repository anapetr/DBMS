//
// Created by Pavel Petkov on 24.07.22.
//

#ifndef DBMS_NODEKEY_H
#define DBMS_NODEKEY_H


#include "TypeWrapper.h"
#include "RecordPtr.h"

/**
 * @brief This is the data that a node of the tree contains
 * typeWrapper is the actual data
 * recordPtr is where to find it in the files
 */
class NodeKey {
public:
    TypeWrapper typeWrapper;
    RecordPtr recordPtr;
};


#endif //DBMS_NODEKEY_H
