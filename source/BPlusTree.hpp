#include "struct.hpp"

class BPlusTree{
public:
    BPlusTree(int M);
    Node* FindLeaf(int key, Node* current);
    void Insert(int key, Record* value);
    void InsertRecord(int key, Record* record, Node* parent);
    void InsertNode(int key, Node* child, Node* parent);
    void PrintTreeByBfs();
    
    bool debug = false;
private:
    
    int M;
    Node* root;
};





