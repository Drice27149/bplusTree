struct Record{
    int key;
    int value;
    Record(int key, int value):
    key(key), value(value){}
    ~Record(){}
};

struct Node {
    int* key;
    int Key;
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
    
    // split leaf and return new leaf, size of self will be updated
    Node* SplitLeaf();
    
    void PrintNode();
    void SimplePrint();
};
