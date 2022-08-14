#include "../headers/BPlusTreeNode.h"


int BPlusTreeNode::keyIndex(const TypeWrapper &key) {
    for (int i = 0; i < nodeKeys.size(); i++) {
        if (nodeKeys[i].typeWrapper == key) {
            return i;
        }
    }
    return -1;
}

BPlusTreeNode::BPlusTreeNode(int order, bool isLeaf) {
    this->setIsLeaf(isLeaf);
    this->setOrder(order);

    for (size_t i = 0; i < order + 1; i++) {
        children.push_back(nullptr);
    }
}

int BPlusTreeNode::getOrder() const {
    return order;
}

void BPlusTreeNode::setOrder(int order) {
    BPlusTreeNode::order = order;
}

bool BPlusTreeNode::isLeaf() const {
    return leaf;
}

void BPlusTreeNode::setIsLeaf(bool isLeaf) {
    BPlusTreeNode::leaf = isLeaf;
}

std::vector<NodeKey> &BPlusTreeNode::getNodeKeys() {
    return nodeKeys;
}

void BPlusTreeNode::setKeys(const std::vector<NodeKey> &keys) {
    BPlusTreeNode::nodeKeys = keys;
}

std::vector<BPlusTreeNode *> &BPlusTreeNode::getChildren() {
    return children;
}

void BPlusTreeNode::setPtr(const std::vector<BPlusTreeNode *> &ptr) {
    BPlusTreeNode::children = ptr;
}
