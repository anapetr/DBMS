#include "../headers/BPlusTree.h"

BPlusTree::BPlusTree() : order(3), size(0), root(nullptr) {}

BPlusTree::BPlusTree(std::ifstream &in) {
    root = nullptr;
    in.read((char *) &order, sizeof(order));
    in.read((char *) &size, sizeof(size));
    int tmpSize = size;
    for (int i = 0; i < tmpSize; i++) {
        this->insert({TypeWrapper(in), RecordPtr(in)});
    }
    size = tmpSize;
}

/**
 * Inserts data in the tree if it can be inserted in any of the leaves
 * if it can NOT be inserted in leaf and new nodes have to be constructed
 * BPlusTree::insertInternal is called
 * @param nodeKey the data to be inserted
 */
void BPlusTree::insert(NodeKey nodeKey) {
    if (root == nullptr) {
        root = new BPlusTreeNode(order, true);
        root->getNodeKeys().push_back(nodeKey);
    } else {
        BPlusTreeNode *cursor = root;
        BPlusTreeNode *parent = nullptr;

        //while cursor is NOT leaf traverse down until leaf is found
        while (!cursor->isLeaf()) {
            parent = cursor;
            for (int i = 0; i < cursor->getNodeKeys().size(); i++) {
                if (nodeKey.typeWrapper < cursor->getNodeKeys()[i].typeWrapper) {
                    cursor = cursor->getChildren()[i];
                    break;
                }

                if (i == cursor->getNodeKeys().size() - 1) {
                    cursor = cursor->getChildren()[i + 1];
                    break;
                }
            }
        }

        //when leaf is found:
        if (cursor->getNodeKeys().size() < order) {
            //if the number of data records it contains are less that order, insert new data
            long pos = 0;
            std::vector<NodeKey> cursorKeys = cursor->getNodeKeys();
            while (pos < cursorKeys.size() && nodeKey.typeWrapper > cursorKeys[pos].typeWrapper) {
                pos++;
            }

            cursor->getNodeKeys().insert(cursor->getNodeKeys().begin() + pos, nodeKey);
            cursor->getChildren()[cursor->getNodeKeys().size()] = cursor->getChildren()[cursor->getNodeKeys().size() -
                                                                                        1];
            cursor->getChildren()[cursor->getNodeKeys().size() - 1] = nullptr;
        } else {
            //otherwise, break leaf

            BPlusTreeNode *newLeaf = splitNode(cursor, nodeKey);

            if (cursor == root) {
                BPlusTreeNode *newRoot = new BPlusTreeNode(order, false);
                newRoot->getNodeKeys().push_back(newLeaf->getNodeKeys().front());
                newRoot->getChildren()[0] = cursor;
                newRoot->getChildren()[1] = newLeaf;
                root = newRoot;
            } else {
                insertInternal(newLeaf->getNodeKeys()[0], parent, newLeaf);
            }
        }
    }
    size++;
}

void BPlusTree::remove(const TypeWrapper &valueToRemove) {
    if (!root) {
        return;
    }
    BPlusTreeNode *cursor = root;
    BPlusTreeNode *parent = nullptr;
    int leftSibling, rightSibling;
    // find the leaf BPlusTreeNode containing the nodeKey
    while (!cursor->isLeaf()) {
        for (int i = 0; i < cursor->getNodeKeys().size(); i++) {
            parent = cursor;
            leftSibling = i - 1;
            rightSibling = i + 1;
            if (valueToRemove < cursor->getNodeKeys()[i].typeWrapper) {
                cursor = cursor->getChildren()[i];
                break;
            }
            if (i == cursor->getNodeKeys().size() - 1) {
                leftSibling = i;
                rightSibling = i + 2;
                cursor = cursor->getChildren()[i + 1];
                break;
            }
        }
    }

    // in the BPlusTreeNode, find the kvp, if it exists
    bool found = false;
    int pos;
    for (pos = 0; pos < cursor->getNodeKeys().size(); pos++) {
        if (cursor->getNodeKeys()[pos].typeWrapper == valueToRemove) {
            found = true;
            break;
        }
    }
    if (!found) {
        return;
    }

    // erase the key from the BPlusTreeNode's keys
    cursor->getNodeKeys().erase(cursor->getNodeKeys().begin() + pos);
    size--;

    // in case we are deleting the only element in the tree, just delete the tree itself
    if (cursor == root) {
        for (int i = 0; i < order + 1; i++) {
            cursor->getChildren()[i] = nullptr;
        }

        if (cursor->getNodeKeys().size() == 0) {
            delete cursor;
            root = nullptr;
        }
        return;
    }

    // adjust the child pointers after deleting the element
    cursor->getChildren()[cursor->getNodeKeys().size()] = cursor->getChildren()[cursor->getNodeKeys().size() + 1];
    cursor->getChildren()[cursor->getNodeKeys().size() + 1] = nullptr;

    if (cursor->getNodeKeys().size() >= (order + 1) / 2 - 1 /*delete -1*/) {
        removeIndex(valueToRemove, parent);
        return;
    }

    // Borrow
    if (leftSibling >= 0) {
        BPlusTreeNode *leftNode = parent->getChildren()[leftSibling];
        if (leftNode->getNodeKeys().size() >= (order + 1) / 2 /*+ 1*/) {
            // take the last element from left sibling and insert it in cursor's start + readjust pointer to point to next leaf
            cursor->getNodeKeys().insert(cursor->getNodeKeys().begin(), leftNode->getNodeKeys().back());
            cursor->getChildren()[cursor->getNodeKeys().size()] = cursor->getChildren()[cursor->getNodeKeys().size() -
                                                                                        1];
            cursor->getChildren()[cursor->getNodeKeys().size() - 1] = nullptr;

            // after inserting last el. from leftSibling in cursor, erase it from left sibl. and readjust pointers
            leftNode->getNodeKeys().erase(leftNode->getNodeKeys().end() - 1);
            leftNode->getChildren()[leftNode->getNodeKeys().size()] = cursor;
            leftNode->getChildren()[leftNode->getNodeKeys().size() + 1] = nullptr;
            parent->getNodeKeys()[leftSibling] = cursor->getNodeKeys()[0];
            removeIndex(valueToRemove, parent);
            return;
        }
    }

    //Borrow
    if (rightSibling <= parent->getNodeKeys().size()) {
        BPlusTreeNode *rightNode = parent->getChildren()[rightSibling];
        if (rightNode->getNodeKeys().size() >= (order + 1) / 2 /*+ 1*/) {
            // Borrow key from right sibling and readjust pointers
            cursor->getNodeKeys().push_back(rightNode->getNodeKeys()[0]);
            cursor->getChildren()[cursor->getNodeKeys().size()] = cursor->getChildren()[cursor->getNodeKeys().size() -
                                                                                        1];
            cursor->getChildren()[cursor->getNodeKeys().size() - 1] = nullptr;

            // Erase the borrowed element and readjust nodes
            rightNode->getNodeKeys().erase(rightNode->getNodeKeys().begin());
            rightNode->getChildren()[rightNode->getNodeKeys().size()] = rightNode->getChildren()[
                    rightNode->getNodeKeys().size() + 1];
            rightNode->getChildren()[rightNode->getNodeKeys().size() + 1] = nullptr;
            parent->getNodeKeys()[rightSibling -
                                  1] = rightNode->getNodeKeys()[0]; // to fulfil the properties for b+tree, we take the smallest element
            // from the right sibling and put it in the parent's keys
            removeIndex(valueToRemove, parent);
            return;
        }
    }

    // Merges
    if (leftSibling >= 0) {
        BPlusTreeNode *leftNode = parent->getChildren()[leftSibling];
        leftNode->getChildren()[leftNode->getNodeKeys().size()] = nullptr;
        for (int j = 0; j < cursor->getNodeKeys().size(); j++)
            leftNode->getNodeKeys().push_back(cursor->getNodeKeys()[j]);

        leftNode->getChildren()[leftNode->getNodeKeys().size()] = cursor->getChildren()[cursor->getNodeKeys().size()];
        // Merging two leaf nodes
        removeInternal(parent->getNodeKeys()[leftSibling].typeWrapper, parent, cursor);
        delete cursor;
    } else if (rightSibling <= parent->getNodeKeys().size()) {
        BPlusTreeNode *rightNode = parent->getChildren()[rightSibling];
        cursor->getChildren()[cursor->getNodeKeys().size()] = nullptr;
        for (int i = cursor->getNodeKeys().size(), j = 0; j < rightNode->getNodeKeys().size(); i++, j++)
            cursor->getNodeKeys().insert(cursor->getNodeKeys().begin() + i, rightNode->getNodeKeys()[j]);

        cursor->getChildren()[cursor->getNodeKeys().size()] = rightNode->getChildren()[rightNode->getNodeKeys().size()];
        // Merging two leaf nodes
        removeInternal(parent->getNodeKeys()[rightSibling - 1].typeWrapper, parent, rightNode);
        delete rightNode;
    }
    removeIndex(valueToRemove, parent);
}

BPlusTreeNode *BPlusTree::splitNode(BPlusTreeNode *&cursor, NodeKey &nodeKey) {
    //create new BPlusTreeNode that is leaf and copy all the data from the current from cursor
    BPlusTreeNode *newLeaf = new BPlusTreeNode(order, true);
    std::vector<NodeKey> virtualNode;
    //+1 for the nodeKey
    virtualNode.reserve(order + 1);
    virtualNode.insert(virtualNode.begin(), cursor->getNodeKeys().begin(), cursor->getNodeKeys().end());

    int i = 0;
    while (i < order && nodeKey.typeWrapper > virtualNode[i].typeWrapper) {
        i++;
    }
    virtualNode.insert(virtualNode.begin() + i, nodeKey);

    std::vector<NodeKey> cursorKeys = cursor->getNodeKeys();
    cursorKeys.clear();
    cursorKeys.reserve((order + 1) / 2);
    cursorKeys.insert(cursor->getNodeKeys().begin(), virtualNode.begin(), virtualNode.begin() + (order + 1) / 2);

    int newLeafKeysSize = order + 1 - (order + 1) / 2;
    std::vector<NodeKey> newLeafKeys = newLeaf->getNodeKeys();
    newLeafKeys.reserve(newLeafKeysSize);
    newLeafKeys.insert(newLeaf->getNodeKeys().begin(), virtualNode.begin() + (order + 1) / 2, virtualNode.end());

    cursor->getChildren()[cursorKeys.size()] = newLeaf;
    newLeaf->getChildren()[newLeafKeys.size()] = cursor->getChildren()[order];
    cursor->getChildren()[order] = nullptr;

    return newLeaf;
}

void BPlusTree::insertInternal(NodeKey nodeKey, BPlusTreeNode *cursor, BPlusTreeNode *child) {

    if (cursor->getNodeKeys().size() < order) {
        int i = 0;
        while (i < cursor->getNodeKeys().size() && nodeKey.typeWrapper > cursor->getNodeKeys()[i].typeWrapper)
            i++;

        // Shift child pointers with 1 to the right
        for (int j = cursor->getNodeKeys().size() + 1; j > i + 1; j--)
            cursor->getChildren()[j] = cursor->getChildren()[j - 1];

        cursor->getNodeKeys().insert(cursor->getNodeKeys().begin() + i, nodeKey);
        cursor->getChildren()[i + 1] = child;
    } else {
        BPlusTreeNode *newInternal = new BPlusTreeNode(order,
                                                       false); // create new internal BPlusTreeNode when splitting
        std::vector<NodeKey> virtualNodeKey;
        std::vector<BPlusTreeNode *> virtualPtr;
        virtualNodeKey.reserve(order +
                               1); // we assume the nodeKeys are full so we reserve exactly fOrder+1 so we can split accordingly to the newly added el.
        virtualPtr.reserve(order +
                           2); // if nodeKeys are full of an internal BPlusTreeNode, that means children is also full so we reserve fOrder+2

        virtualNodeKey.insert(virtualNodeKey.begin(), cursor->getNodeKeys().begin(),
                              cursor->getNodeKeys().end()); // fill with cursor's half
        virtualPtr.insert(virtualPtr.begin(), cursor->getChildren().begin(),
                          cursor->getChildren().end()); // fill with cursor's half

        int i = 0;
        while (i < order && nodeKey.typeWrapper > virtualNodeKey[i].typeWrapper) // find the place for the new key
            i++;

        virtualNodeKey.insert(virtualNodeKey.begin() + i, nodeKey); // insert new key
        virtualPtr.insert(virtualPtr.begin() + i + 1, child); // insert the element from the previous iteration

        // We are halving the nodeKeys and pointers of cursor, because we are splitting it
        int k = cursor->getChildren().size() - 1;
        while (cursor->getNodeKeys().size() != (order + 1) / 2) {
            cursor->getNodeKeys().pop_back();
            cursor->getChildren()[k] = nullptr;
            k--;
        }

        // Since we dont need repeating elements in the internal nodes, we skip the typeWrapper key here by saying fOrder + 1 - (fOrder+1)/2 - 1
        int newInternalKeysSize = order - (order + 1) / 2;
        newInternal->getNodeKeys().reserve(newInternalKeysSize);
        newInternal->getNodeKeys().insert(newInternal->getNodeKeys().begin(),
                                          virtualNodeKey.begin() + cursor->getNodeKeys().size() + 1,
                                          virtualNodeKey.end()); // Fill nodeKeys of newInternal
        for (int j = 0, i = cursor->getNodeKeys().size() + 1; i < virtualPtr.size(); i++, j++) {
            newInternal->getChildren()[j] = virtualPtr[i];
        }

        if (cursor == root) {
            BPlusTreeNode *newRoot = new BPlusTreeNode(order, false);
            newRoot->getNodeKeys().push_back(virtualNodeKey[cursor->getNodeKeys().size()]);
            newRoot->getChildren()[0] = cursor;
            newRoot->getChildren()[1] = newInternal;
            root = newRoot;
        } else {
            insertInternal(virtualNodeKey[cursor->getNodeKeys().size()], findParent(root, cursor), newInternal);
        }
    }
}

BPlusTreeNode *BPlusTree::findParent(BPlusTreeNode *root, BPlusTreeNode *child) {
    if (child == root || root == nullptr) {
        return nullptr;
    }

    BPlusTreeNode *parent = nullptr;
    if (root->isLeaf() || (root->getChildren()[0])->isLeaf()) {
        return nullptr;
    }

    for (BPlusTreeNode *rootChild: root->getChildren()) {
        if (rootChild == child) {
            return root;
        } else {
            parent = findParent(rootChild, child);
            if (parent) {
                return parent;
            }
        }
    }
    return parent;
}

void BPlusTree::removeInternal(const TypeWrapper &valueToRemove, BPlusTreeNode *&cursor, BPlusTreeNode *&child) {
    if (cursor == root) {
        if (cursor->getNodeKeys().size() == 1) {
            if (cursor->getChildren()[1] == child) {
                // Changing root BPlusTreeNode
                delete child;
                root = cursor->getChildren()[0];
                delete cursor;
                cursor = child = nullptr;
                return;
            } else if (cursor->getChildren()[0] == child) {
                // Changing root BPlusTreeNode
                delete child;
                root = cursor->getChildren()[1];
                delete cursor;
                cursor = child = nullptr;
                return;
            }
        }
    }

    int pos;
    // find pos of child that is to be deleted
    for (pos = 0; pos < cursor->getNodeKeys().size() + 1; pos++) {
        if (cursor->getChildren()[pos] == child) {
            break;
        }
    }

    // shift pointers one to the left so we "eat" the child that is to be deleted
    for (int i = pos; i < cursor->getNodeKeys().size() + 1; i++) {
        if (i != cursor->getNodeKeys().size()) {
            cursor->getChildren()[i] = cursor->getChildren()[i + 1];
        } else {
            cursor->getChildren()[i] = nullptr;
        }
    }

    for (pos = 0; pos < cursor->getNodeKeys().size(); pos++) {
        if (cursor->getNodeKeys()[pos].typeWrapper == valueToRemove) {
            break;
        }
    }

    // Erase the valueToRemove that we have sent up the tree
    cursor->getNodeKeys().erase(cursor->getNodeKeys().begin() + pos);

    if (cursor->getNodeKeys().size() >= (order + 1) / 2 - 1) {
        return;
    }
    if (cursor == root) {
        return;
    }

    BPlusTreeNode *parent = findParent(root, cursor);
    int leftSibling = -1, rightSibling = parent->getNodeKeys().size() + 1;
    for (pos = 0; pos < parent->getNodeKeys().size() + 1; pos++) {
        if (parent->getChildren()[pos] == cursor) {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }

    if (leftSibling >= 0) {
        BPlusTreeNode *leftNode = parent->getChildren()[leftSibling];
        if (leftNode->getNodeKeys().size() >= (order + 1) / 2) {
            cursor->getNodeKeys().insert(cursor->getNodeKeys().begin(), parent->getNodeKeys()[leftSibling]);
            parent->getNodeKeys()[leftSibling] = leftNode->getNodeKeys()[leftNode->getNodeKeys().size() - 1];
            for (int i = cursor->getNodeKeys().size() + 1; i > 0; i--) {
                cursor->getChildren()[i] = cursor->getChildren()[i - 1];
            }

            cursor->getChildren()[0] = leftNode->getChildren()[leftNode->getNodeKeys().size()];
            leftNode->getChildren()[leftNode->getNodeKeys().size()] = nullptr;
            leftNode->getNodeKeys().pop_back();
            return;
        }
    }

    if (rightSibling <= parent->getNodeKeys().size()) {
        BPlusTreeNode *rightNode = parent->getChildren()[rightSibling];
        if (rightNode->getNodeKeys().size() >= (order + 1) / 2) {
            cursor->getNodeKeys().push_back(parent->getNodeKeys()[pos]);
            parent->getNodeKeys()[pos] = rightNode->getNodeKeys()[0];
            cursor->getChildren()[cursor->getNodeKeys().size()/*delete +1*/] = rightNode->getChildren()[0];

            for (int i = 0; i < rightNode->getNodeKeys().size(); ++i) {
                rightNode->getChildren()[i] = rightNode->getChildren()[i + 1];
            }

            rightNode->getChildren()[rightNode->getNodeKeys().size()] = nullptr;
            rightNode->getNodeKeys().erase(rightNode->getNodeKeys().begin());
            return;
        }
    }

    if (leftSibling >= 0) {
        BPlusTreeNode *leftNode = parent->getChildren()[leftSibling];
        leftNode->getNodeKeys().push_back(parent->getNodeKeys()[leftSibling]);

        for (int i = leftNode->getNodeKeys().size(), j = 0;
             i < order + 1 && j < cursor->getNodeKeys().size() + 1; j++, i++) {
            leftNode->getChildren()[i] = cursor->getChildren()[j];
            cursor->getChildren()[j] = nullptr;
        }

        for (int j = 0; j < cursor->getNodeKeys().size(); j++) {
            leftNode->getNodeKeys().push_back(cursor->getNodeKeys()[j]);
        }

        removeInternal(parent->getNodeKeys()[leftSibling].typeWrapper, parent, cursor);
    } else if (rightSibling <= parent->getNodeKeys().size()) {
        BPlusTreeNode *rightNode = parent->getChildren()[rightSibling];
        cursor->getNodeKeys().push_back(parent->getNodeKeys()[rightSibling - 1]);

        for (int i = cursor->getNodeKeys().size(), j = 0;
             i < order + 1 && j < rightNode->getNodeKeys().size() + 1; j++, i++) {
            cursor->getChildren()[i] = rightNode->getChildren()[j];
            rightNode->getChildren()[j] = nullptr;
        }

        for (int j = 0; j < rightNode->getNodeKeys().size(); j++) {
            cursor->getNodeKeys().push_back(rightNode->getNodeKeys()[j]);
        }
        removeInternal(parent->getNodeKeys()[rightSibling - 1].typeWrapper, parent, rightNode);
    }
}

void BPlusTree::removeIndex(const TypeWrapper &key, BPlusTreeNode *cursor) {
    if (!cursor) {
        return;
    }
    int indKvp = cursor->keyIndex(key);
    if (indKvp == -1) {
        removeIndex(key, findParent(root, cursor));
    } else {
        cursor->getNodeKeys()[indKvp] = getSmallestElementInSubTree(cursor->getChildren()[indKvp + 1]);
    }
}

NodeKey BPlusTree::getSmallestElementInSubTree(BPlusTreeNode *root) {
    while (!root->isLeaf()) {
        root = root->getChildren()[0];
    }

    if (!root) {
        return {-1, {-1, -1}};
    }
    return root->getNodeKeys().front();
}

BPlusTreeNode *BPlusTree::search(const TypeWrapper &key) {
    if (root == nullptr) {
        return nullptr;
    }

    BPlusTreeNode *cursor = root;
    while (!cursor->isLeaf()) {
        for (int i = 0; i < cursor->getNodeKeys().size(); i++) {
            if (key < cursor->getNodeKeys()[i].typeWrapper) {
                cursor = cursor->getChildren()[i];
                break;
            }
            if (i == cursor->getNodeKeys().size() - 1) {
                cursor = cursor->getChildren()[i + 1];
                break;
            }
        }
    }

    for (int i = 0; i < cursor->getNodeKeys().size(); i++) {
        if (cursor->getNodeKeys()[i].typeWrapper == key) {
            return cursor;
        }
    }
    return nullptr;
}

RecordPtr BPlusTree::getRecordAtIndex(const TypeWrapper &key) {
    BPlusTreeNode *target = search(key);
    return target->getNodeKeys()[target->keyIndex(key)].recordPtr;
}


std::vector<RecordPtr> BPlusTree::getRecordsFromQuery(InternalQuery &query) {
    if (!query.isPrimaryKeyQuery())
        throw std::invalid_argument("Cannot select/remove items from tree without primary index");

    std::vector<RecordPtr> answer;
    if (query.getOperator() == Operation::EQUAL) {
        BPlusTreeNode *targetNode = search(query.getValue());
        if (targetNode) {
            answer.push_back(targetNode->getNodeKeys()[targetNode->keyIndex(query.getValue())].recordPtr);
        }
    } else if (query.getOperator() == Operation::NOT_EQUAL) {
        answer = getAllRecordPtrsExcept(query.getValue());
    } else if (query.getOperator() == Operation::GREATER_THAN) {
        answer = getRecordPtrsGreaterThan(query.getValue(), false);
    } else if (query.getOperator() == Operation::GREATER_THAN_OR_EQUAL) {
        answer = getRecordPtrsGreaterThan(query.getValue(), true);
    } else if (query.getOperator() == Operation::LESS_THAN) {
        answer = getRecordPtrsLessThan(query.getValue(), false);
    } else if (query.getOperator() == Operation::LESS_THAN_OR_EQUAL) {
        answer = getRecordPtrsLessThan(query.getValue(), true);
    }

    return answer;
}

std::vector<RecordPtr> BPlusTree::getAllRecordPtrsExcept(const TypeWrapper &except) {
    std::vector<RecordPtr> answer;
    BPlusTreeNode *cursor = root;
    while (cursor && !cursor->isLeaf()) {
        cursor = cursor->getChildren()[0];
    }

    while (cursor) {
        for (int i = 0; i < cursor->getNodeKeys().size(); i++) {
            if (cursor->getNodeKeys()[i].typeWrapper == except) {
                continue;
            }
            answer.push_back(cursor->getNodeKeys()[i].recordPtr);
        }
        cursor = cursor->getChildren()[cursor->getNodeKeys().size()];
    }
    return answer;
}

std::vector<RecordPtr> BPlusTree::getRecordPtrsGreaterThan(const TypeWrapper &what, bool orEqual) {
    std::vector<RecordPtr> answer;
    BPlusTreeNode *cursor = root;
    while (cursor && !cursor->isLeaf()) {
        for (int i = 0; i < cursor->getNodeKeys().size(); i++) {
            if (cursor->getNodeKeys()[i].typeWrapper > what) {
                cursor = cursor->getChildren()[i];
                break;
            }
            if (i == cursor->getNodeKeys().size() - 1) {
                cursor = cursor->getChildren()[i + 1];
                break;
            }
        }
    }

    while (cursor) {
        for (int i = 0; i < cursor->getNodeKeys().size(); i++) {
            if (orEqual && cursor->getNodeKeys()[i].typeWrapper >= what) {
                answer.push_back(cursor->getNodeKeys()[i].recordPtr);
            } else if (!orEqual && cursor->getNodeKeys()[i].typeWrapper > what) {
                answer.push_back(cursor->getNodeKeys()[i].recordPtr);
            }
        }
        cursor = cursor->getChildren()[cursor->getNodeKeys().size()];
    }
    return answer;
}

std::vector<RecordPtr> BPlusTree::getRecordPtrsLessThan(const TypeWrapper &what, bool orEqual) {
    std::vector<RecordPtr> answer;
    BPlusTreeNode *cursor = root;
    while (cursor && !cursor->isLeaf()) {
        cursor = cursor->getChildren()[0];
    }

    while (cursor) {
        for (int i = 0; i < cursor->getNodeKeys().size(); i++) {
            if (orEqual && cursor->getNodeKeys()[i].typeWrapper <= what) {
                answer.push_back(cursor->getNodeKeys()[i].recordPtr);
            } else if (!orEqual && cursor->getNodeKeys()[i].typeWrapper < what) {
                answer.push_back(cursor->getNodeKeys()[i].recordPtr);
            }
        }
        cursor = cursor->getChildren()[cursor->getNodeKeys().size()];
    }
    return answer;
}

//Root Left Right
void BPlusTree::saveTreeInFile(std::ofstream &out) {
    std::set<TypeWrapper> visited;
    out.write((char *) &order, sizeof(order));
    out.write((char *) &size, sizeof(size));
    saveTreeInFileHelper(root, out, visited);
}

void BPlusTree::saveTreeInFileHelper(BPlusTreeNode *cursor, std::ofstream &out, std::set<TypeWrapper> &visited) {
    if (cursor) {
        for (int i = 0; i < cursor->getNodeKeys().size(); i++) {
            if (visited.find(cursor->getNodeKeys()[i].typeWrapper) != visited.end()) {
                continue;
            }

            cursor->getNodeKeys()[i].typeWrapper.write(out);
            cursor->getNodeKeys()[i].recordPtr.write(out);
            visited.insert(cursor->getNodeKeys()[i].typeWrapper);
        }
        if (!cursor->isLeaf()) {
            for (int i = 0; i < cursor->getNodeKeys().size() + 1; i++) {
                saveTreeInFileHelper(cursor->getChildren()[i], out, visited);
            }
        }
    }
}