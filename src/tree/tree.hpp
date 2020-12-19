#include "struct.hpp"

class tree{
public:
    tree(int M);
    Node* FindLeaf(int key, Node* current);
    Record* FindRecord(int key);
    
    void Insert(int key, Record* value);
    // children's parent pointers are set up when inserting into parent
    void InsertRecord(int key, Record* record, Node* parent);
    void InsertNode(int key, Node* child, Node* parent);
    
    void Delete(int key);
    void DeleteRecord(int key, Node* node);
    void DeleteNode(Node* deleteNode, Node* node);

    void PrintTreeByBfs();
    
    bool debug = false;
private:
    
    int M;
    Node* root;
};





