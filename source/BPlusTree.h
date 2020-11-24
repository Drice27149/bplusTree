struct Node {
    int key;
    Node* parent;
    Node* pre;
    Node* next;
    Node* firstChild;
    int childrenCount;
    bool isLeaf;
    Node(): key(-1), parent(nullptr), pre(nullptr), next(nullptr), firstChild(nullptr), childrenCount(0), isLeaf(false){} 
};

class BPlusTree{
public:
    void insert(Node* newNode, Node* current);
    
private:
    int M; // max number of children in an internal node
    int L; // max number of data in a leaf node 
    Node* root;
};


void BPlusTree::insert(Node* newNode, Node* current){
    if(current->isLeaf){
        if(current->firstChild && newNode->key > current->firstChild->key){
            for(Node* child = current->firstChild; child; child = child->next){
                if(!(child->next) || (child->next->key > newNode->key)){
                    newNode->pre = child;
                    newNode->next = child->next;
                    newNode->parent = current;
                    child->next = newNode;
                    child->next->pre = newNode;
                    current->childrenCount++;
                    break;
                }
            }
        }
        else{
            newNode->pre = nullptr;
            newNode->next = current->firstChild;
            newNode->parent = current;
            current->childrenCount++;
            current->firstChild = newNode;
        }
        
        if(current->childrenCount > L){
            //todo: split
        }
    }
    else{
        
    }
}
