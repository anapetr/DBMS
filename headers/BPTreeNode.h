//
// Created by Pavel Petkov on 24.07.22.
//

#ifndef DBMS_BPTREENODE_H
#define DBMS_BPTREENODE_H

#include <string>
#include <vector>
#include "./RecordPtr.h"
#include "./TypeWrapper.h"
#include "NodeKey.h"

/**
 * @brief Node of the B+ tree
 */
class BPTreeNode {
private:
    int order;
    bool leaf;
    std::vector<NodeKey> keys;
    std::vector<BPTreeNode*> ptr;

public:
    BPTreeNode(int order, bool isLeaf);

    int keyIndex(const TypeWrapper &key);

    int getOrder() const;

    void setOrder(int order);

    bool isLeaf() const;

    void setIsLeaf(bool isLeaf);

    std::vector<NodeKey> &getKeys();

    void setKeys(const std::vector<NodeKey> &keys);

    std::vector<BPTreeNode *> &getPtr();

    void setPtr(const std::vector<BPTreeNode *> &ptr);
};

#endif //DBMS_BPTREENODE_H
