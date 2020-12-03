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
    int size; // number of children 
    bool isLeaf;
    
    Node(int M, bool isLeaf);
    ~Node();
    void PrintNode();
};
