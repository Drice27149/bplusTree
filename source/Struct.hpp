struct Record{
    int key;
    int value;
    Record(int key, int value):
    key(key), value(value){}
};

struct Node {
    int* key;
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
    
    Node* PopFrontNode();
    Node* PopBackNode();
    void PushFrontNode(int key, Node* node);
    void PushBackNode(int key, Node* node);
    
    void PrintNode();
};
