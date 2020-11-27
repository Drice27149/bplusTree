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
    Node* FindLeaf(int key, Node* current);
    void Insert(int key, Node* newNode);
    void InsertRecordtoLeaf(int key, Node* record, Node* leaf);
    void InsertNodetoInternal(Node* newNode, Node* current);
    
    
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
       
    }
    else{
        //error
    }
}

void BPlusTree::InsertRecordtoLeaf(int key, Node* record,Node* leaf){
     if(key < leaf->firstChild->key){
         record->next = leaf->firstChild;
         record->pre = nullptr;
         leaf->firstChild = record;
     }
     else{
         for(Node* child = leaf->firstChild; child; child = child->next){
            if(!child->next || key < child->next->key){
                record->pre = child;
                record->next = child->next;
                break;
            }
         }
     }
     record->parent = leaf;
     leaf->size++;
     if(leaf->size > M){
         //split 
        Node* lastLeft = leaf->firstChild;
        int leftSize = (M+1)/2;
        for(int i = 0; i < leftSize-1; i++) lastLeft = lastLeft->next;
        Node* splitRightNode = new Node();
        splitRightNode->key = lastLeft->next->key;
        splitRightNode->firstChild = lastLeft->next;
        splitRightNode->size = M+1-leftSize;
        splitRightNode->isLeaf = true;
        if(leaf->parent){
            InsertNodetoInternal(splitRightNode, leaf->parent);
        }
        else{
            //height of tree will increase by one
            //create new root
        }
     }
}

void BPlusTree::InsertNodetoInternal(Node* newNode, Node* current){
    
}
