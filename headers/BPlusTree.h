//
// Created by Pavel Petkov on 24.07.22.
//

#ifndef DBMS_BPLUSTREE_H
#define DBMS_BPLUSTREE_H

#include <fstream>
#include <set>
#include "./BPlusTreeNode.h"
#include "InternalQuery.h"

class BPlusTree {
public:
    BPlusTree();

    BPlusTree(std::ifstream &in);

    void insert(NodeKey keyValuePair);

    BPlusTreeNode *splitNode(BPlusTreeNode *&cursor, NodeKey &nodeKey);

    void remove(const TypeWrapper &valueToRemove);

    BPlusTreeNode *search(const TypeWrapper &key);

    RecordPtr getRecordAtIndex(const TypeWrapper &key);

    std::vector<RecordPtr> getRecordsFromQuery(InternalQuery &query);

    std::vector<RecordPtr> getAllRecordPtrsExcept(const TypeWrapper& except);

    std::vector<RecordPtr> getRecordPtrsGreaterThan(const TypeWrapper& what, bool orEqual);

    std::vector<RecordPtr> getRecordPtrsLessThan(const TypeWrapper& what, bool orEqual);

    void saveTreeInFile(std::ofstream& out);

private:
    int order;
    std::size_t size;
    BPlusTreeNode *root;

    void insertInternal(NodeKey nodeKey, BPlusTreeNode *cursor, BPlusTreeNode *child);

    BPlusTreeNode *findParent(BPlusTreeNode *root, BPlusTreeNode *child);

    void removeInternal(const TypeWrapper &valueToRemove, BPlusTreeNode *&cursor, BPlusTreeNode *&child);

    void removeIndex(const TypeWrapper &key, BPlusTreeNode *cursor);

    NodeKey getSmallestElementInSubTree(BPlusTreeNode *root);

    void saveTreeInFileHelper(BPlusTreeNode* cursor, std::ofstream& out, std::set<TypeWrapper>& visited);

};

#endif //DBMS_BPLUSTREE_H
