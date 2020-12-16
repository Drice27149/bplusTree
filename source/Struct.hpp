struct Record{
    int key;
    int value;
    Record(int key, int value):
    key(key), value(value){}
    ~Record(){}
};

struct Node {
    int* key;
    int KEY;
    int M;
    Node** children;
    Record** records;
    Node* parent;
    Node* neighbor;
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
