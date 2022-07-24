//
// Created by Pavel Petkov on 24.07.22.
//

#ifndef DBMS_BPTREE_H
#define DBMS_BPTREE_H

#include <fstream>
#include "./BPTreeNode.h"

class BPTree {

private:
    int order;
    std::size_t size;
    BPTreeNode* root;

public:
    BPTree(std::ifstream& in);
    void insert(NodeKey keyValuePair);
    BPTreeNode* splitNode(BPTreeNode*& cursor, NodeKey& nodeKey);
};




#endif //DBMS_BPTREE_H
