#include<cstdio>
#include<queue>
/*
struct Node {
    int key;
    Node* parent;
    Node* pre;
    Node* next;
    Node* firstChild;
    int size; // number of children
    bool isLeaf;
    int id;
    Node(): key(-1), parent(nullptr), pre(nullptr), next(nullptr), firstChild(nullptr), size(0), isLeaf(false){}
};

class BPlusTree{
public:
    BPlusTree(int m);
    void Insert(int key);
    void PrintTree(); // mainly use to test and debug
    
private:
    Node* FindLeaf(int key, Node* current);
    void InsertChildToParent(Node* childNode, Node* parentNode);
    Node* CreateNewNode();
   

private:
    int M; // max number of children in a node
    static int totalNode; // next new node's id, increase by one for each new node
    Node* root;
};

int BPlusTree::totalNode = 0;

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

BPlusTree::BPlusTree(int m){
    root = CreateNewNode();
    root->isLeaf = true;
    M = m;
}

void BPlusTree::Insert(int key){
    Node* leaf = FindLeaf(key, root);
    if(leaf){
        Node* newNode = CreateNewNode();
        newNode->key = key;
        InsertChildToParent(newNode, leaf);
    }
}

void BPlusTree::InsertChildToParent(Node* childNode, Node* parentNode){
    if(!parentNode->firstChild || childNode->key < parentNode->firstChild->key){
        childNode->pre = nullptr;
        childNode->next = parentNode->firstChild;
        if(parentNode->firstChild){
            parentNode->firstChild->pre = childNode;
        }
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
        Node* splitNode = CreateNewNode();
        splitNode->firstChild = lastLeft->next;
        splitNode->key = lastLeft->next->key;
        splitNode->isLeaf = parentNode->isLeaf;
        lastLeft->next->pre = nullptr;
        lastLeft->next = nullptr;
        parentNode->size = (M+1)/2;
        splitNode->size = M+1 - (M+1)/2;
        if(parentNode->parent){
            InsertChildToParent(splitNode, parentNode->parent);
        }
        else{
            Node* newRoot = CreateNewNode();
            parentNode->parent = splitNode->parent = newRoot;
            parentNode->next = splitNode;
            splitNode->pre = parentNode;
            newRoot->firstChild = parentNode;
            newRoot->size = 1;
            newRoot->isLeaf = false;
            root = newRoot;
        }
    }
}

Node* BPlusTree::CreateNewNode(){
    Node* newNode = new Node();
    newNode->id = ++totalNode;
    return newNode;
}

void BPlusTree::PrintTree(){
    std::queue<Node*>queue;
    queue.push(root);
    while(queue.size()!=0){
        Node* current = queue.front();
        queue.pop();
        //printf the current node
        printf("id = %d, key = %d\nparent = ",current->id,current->key);
        if(current->parent) printf("%d\n",current->parent->id);
        else printf("nullptr\n");
        printf("Size = %d:",current->size);
        for(Node* child = current->firstChild; child; child = child->next){
            printf(" %d",child->id);
        }
        printf("\nKey Sequence: ");
        for(Node* child = current->firstChild; child; child = child->next){
            if(child!=current->firstChild) printf(" %d",child->key);
        }
        printf("\n\n");
        //push child node into queue
        for(Node* child = current->firstChild; child; child = child->next){
            queue.push(child);
        }
    }
}
**/
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
    int size;
    bool isLeaf;
    
    Node(int M, bool isLeaf){
        this->isLeaf = isLeaf;
        this->size = 0;
        key = new int[M-1];
        if(isLeaf){
            records = new Record*[M];
        }
        else{
            children = new Node*[M];
        }
    }
    
    ~Node(){
        if(isLeaf){
            delete[] records;
        }
        else{
            delete[] children;
        }
    }
};
