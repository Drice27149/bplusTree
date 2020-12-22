struct Record{
    int key;
    int value;
    Record* pre;
    Record* next;
    Record(int key, int value):
    key(key), value(value){
        pre = next = nullptr;
    }
    
    // insert self before nextRecord
    void InsertBefore(Record* nextRecord){
        next = nextRecord;
        pre = nextRecord->pre;
        next->pre = this;
        pre->next = this;
    }
    
    ~Record(){}
};

struct Node {
    int KEY;
    int M;
    Node** children;
    Record** records;
    Record* firstRecord; // the first record in the child list, only valid in leaf node
    Node* firstNode; // the first node in the child list, only valid in internal node
    Node* parent;
    Node* pre;
    Node* next;
    
    int size; // number of children 
    bool isLeaf;
    
    Node(int M, bool isLeaf);
    ~Node();
    Record* PopBackRecord();
    Record* PopFrontRecord();
    void PushBackRecord(Record* record);
    void PushFrontRecord(Record* record);
    
    Node* PopFrontNode(int& popKey);
    Node* PopBackNode(int& popKey);
    void PushFrontNode(int key, Node* node);
    void PushBackNode(int key, Node* node);
    
    void ResetChildrenNeighbor();
    
    // split leaf and return the new leaf, size of self will be updated
    Node* SplitLeaf();
    // split node and return the new node, size of self will be updated
    Node* SplitNode();
    // get push up key from full node
    int GetPushUpKey();
    
    void PrintNode();
    void SimplePrint();
};
