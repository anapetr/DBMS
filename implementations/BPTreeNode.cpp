//
// Created by Pavel Petkov on 24.07.22.
//

#include "../headers/BPTreeNode.h"


int BPTreeNode::keyIndex(const TypeWrapper &key) {
    for (int i = 0; i < keys.size(); i++) {
        if (keys[i].typeWrapper == key) {
            return i;
        }
    }

    return -1;
}

BPTreeNode::BPTreeNode(int order, bool isLeaf) {
    this->setIsLeaf(isLeaf);
    this->setOrder(order);

    for (size_t i = 0; i < order + 1; i++) {
        ptr.push_back(nullptr);
    }
}

int BPTreeNode::getOrder() const {
    return order;
}

void BPTreeNode::setOrder(int order) {
    BPTreeNode::order = order;
}

bool BPTreeNode::isLeaf() const {
    return leaf;
}

void BPTreeNode::setIsLeaf(bool isLeaf) {
    BPTreeNode::leaf = isLeaf;
}

std::vector<NodeKey> &BPTreeNode::getKeys() {
    return keys;
}

void BPTreeNode::setKeys(const std::vector<NodeKey> &keys) {
    BPTreeNode::keys = keys;
}

std::vector<BPTreeNode *> &BPTreeNode::getPtr() {
    return ptr;
}

void BPTreeNode::setPtr(const std::vector<BPTreeNode *> &ptr) {
    BPTreeNode::ptr = ptr;
}
