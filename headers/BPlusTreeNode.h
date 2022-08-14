#ifndef DBMS_BPLUSTREENODE_H
#define DBMS_BPLUSTREENODE_H

#include <string>
#include <vector>
#include "./RecordPtr.h"
#include "./TypeWrapper.h"
#include "NodeKey.h"

/**
 * @brief Node of the B+ tree
 */
class BPlusTreeNode {
private:
    int order;
    bool leaf;
    std::vector<NodeKey> nodeKeys;
    std::vector<BPlusTreeNode*> children;

public:
    BPlusTreeNode(int order, bool isLeaf);

    int keyIndex(const TypeWrapper &key);

    int getOrder() const;

    void setOrder(int order);

    bool isLeaf() const;

    void setIsLeaf(bool isLeaf);

    std::vector<NodeKey> &getNodeKeys();

    void setKeys(const std::vector<NodeKey> &keys);

    std::vector<BPlusTreeNode *> &getChildren();

    void setPtr(const std::vector<BPlusTreeNode *> &ptr);
};

#endif //DBMS_BPLUSTREENODE_H
