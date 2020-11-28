struct Node {
    int key;
    Node* parent;
    Node* pre;
    Node* next;
    Node* firstChild;
    int size; // number of children
    bool isLeaf;
    Node(): key(-1), parent(nullptr), pre(nullptr), next(nullptr), firstChild(nullptr), size(0), isLeaf(false){} 
};

class BPlusTree{
public:
    void Insert(int key, Node* newNode);
    
private:
    Node* FindLeaf(int key, Node* current);
    void InsertChildToParent(Node* childNode, Node* parentNode);
    
private:
    int M; // max number of children in a node
    Node* root;
};

Node* BPlusTree::FindLeaf(int key, Node* current){
    if(current->isLeaf){ 
        return current;
    }
    else{
        if(!current->firstChild->next || key < current->firstChild->next->key){
            return FindLeaf(key, current->firstChild);
        }
        else{
            for(Node* child = current->firstChild->next; child; child = child->next){
                if(!child->next || key < child->next->key){
                    return FindLeaf(key, child);
                }
            }
            //not found, error
            return nullptr;
        }
    }
}

void BPlusTree::Insert(int key, Node* newNode){
    Node* leaf = FindLeaf(key, root);
    if(leaf){
       InsertChildToParent(newNode, leaf);
    }
}

void BPlusTree::InsertChildToParent(Node* childNode, Node* parentNode){
    if(!parentNode->firstChild || childNode->key < parentNode->firstChild->key){
        childNode->pre = nullptr;
        childNode->next = parentNode->firstChild;
        parentNode->firstChild->pre = childNode;
        parentNode->firstChild = childNode;
    }
    else{
        for(Node* currentChild = parentNode->firstChild; currentChild; currentChild = currentChild->next){
            if(!currentChild->next || childNode->key < currentChild->next->key){
                childNode->pre = currentChild;
                childNode->next = currentChild->next;
                currentChild->next = childNode;
                break;
            }
        }
    }
    childNode->parent = parentNode;
    parentNode->size++;
    //todo: put the following code into split() fuction
    if(parentNode->size == M+1){
        Node* lastLeft = parentNode->firstChild;
        for(int i = 0; i < (M+1)/2 - 1; i++){
            lastLeft = lastLeft->next;
        }
        Node* splitNode = new Node();
        splitNode->firstChild = lastLeft->next;
        lastLeft->next->pre = nullptr;
        lastLeft->next = nullptr;
        parentNode->size = (M+1)/2;
        splitNode->size = M+1 - (M+1)/2;
        if(parentNode->parent){
            InsertChildToParent(splitNode, parentNode->parent);
        }
        else{
            Node* newRoot = new Node();
            parentNode->parent = splitNode->parent = newRoot;
            parentNode->next = splitNode;
            splitNode->pre = parentNode;
            newRoot->firstChild = parentNode;
            newRoot->size = 1;
        }
    }
}
