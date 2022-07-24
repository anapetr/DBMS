//
// Created by Pavel Petkov on 24.07.22.
//

#include "../headers/BPTree.h"

BPTree::BPTree(std::ifstream &in) {
    root = nullptr;
    in.read((char *) &order, sizeof(order));
    in.read((char *) &size, sizeof(size));
    size_t tmpSize = size;
    for (size_t i = 0; i < tmpSize; i++)
        this->insert({TypeWrapper(in), RecordPtr(in)});

    size = tmpSize;
}

/**
 * Inserts data in the tree if it can be inserted in any of the leaves
 * if it can NOT be inserted in leaf and new nodes have to be constructed
 * BPTree::insertInternal is called
 * @param nodeKey the data to be inserted
 */
void BPTree::insert(NodeKey nodeKey) {
    if (root == nullptr) {
        root = new BPTreeNode(order, true);
        root->getKeys().push_back(nodeKey);
    } else {
        BPTreeNode *cursor = root;
        BPTreeNode *parent = nullptr;

        //while cursor is NOT leaf traverse down until leaf is found
        while (!cursor->isLeaf()) {
            parent = cursor;
            for (int i = 0; i < cursor->getKeys().size(); i++) {
                if (nodeKey.typeWrapper < cursor->getKeys()[i].typeWrapper) {
                    cursor = cursor->getPtr()[i];
                    break;
                }

                if (i == cursor->getKeys().size() - 1) {
                    cursor = cursor->getPtr()[i + 1];
                    break;
                }
            }
        }

        //when leaf is found:
        if (cursor->getKeys().size() < order) {
            //if the number of data records it contains are less that order, insert new data
            long pos = 0;
            std::vector<NodeKey> cursorKeys = cursor->getKeys();
            while (pos < cursorKeys.size() && nodeKey.typeWrapper > cursorKeys[pos].typeWrapper) {
                pos++;
            }

            cursor->getKeys().insert(cursor->getKeys().begin() + pos, nodeKey);
            cursor->getPtr()[cursor->getKeys().size()] = cursor->getPtr()[cursor->getKeys().size() - 1];
            cursor->getPtr()[cursor->getKeys().size() - 1] = nullptr;
        } else {
            //otherwise, break leaf
            BPTreeNode *newLeaf = splitNode(cursor, nodeKey);

            if (cursor == root) {
                BPTreeNode *newRoot = new BPTreeNode(order, false);
                newRoot->getKeys().push_back(newLeaf->getKeys().front());
                newRoot->getPtr()[0] = cursor;
                newRoot->getPtr()[1] = newLeaf;
                root = newRoot;
            } else {
                insertInternal(newLeaf->getPtr()[0], parent, newLeaf);
            }
        }
    }
    size++;

}

BPTreeNode *BPTree::splitNode(BPTreeNode *&cursor, NodeKey &nodeKey) {
    BPTreeNode *newLeaf = new BPTreeNode(order, true);
    std::vector<NodeKey> virtualNode;
    virtualNode.reserve(order + 1);
    virtualNode.insert(virtualNode.begin(), cursor->getKeys().begin(), cursor->getKeys().end());
    
    int i = 0;
    while (i < order && nodeKey.typeWrapper > virtualNode[i].typeWrapper) {
        i++;
    }
    virtualNode.insert(virtualNode.begin() + i, nodeKey);

    std::vector<NodeKey> cursorKeys = cursor->getKeys();
    cursorKeys.clear();
    cursorKeys.reserve((order + 1) / 2);
    cursorKeys.insert(cursor->getKeys().begin(), virtualNode.begin(), virtualNode.begin() + (order + 1) / 2);

    size_t newLeafKeysSize = order + 1 - (order + 1) / 2;
    std::vector<NodeKey> newLeafKeys = newLeaf->getKeys();
    newLeafKeys.reserve(newLeafKeysSize);
    newLeafKeys.insert(newLeaf->getKeys().begin(), virtualNode.begin() + (order + 1) / 2, virtualNode.end());

    cursor->getPtr()[cursorKeys.size()] = newLeaf;
    newLeaf->getPtr()[newLeafKeys.size()] = cursor->getPtr()[order];
    cursor->getPtr()[order] = nullptr;

    return newLeaf;
}
