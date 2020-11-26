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
    
private:
    int M; // max number of children in a node
    Node* root;
};

Node* BPlusTree::FindLeaf(int key, Node* current){
    if(current->isLeaf){ 
        return current;
    }
    else{
        if(key < current->firstChild->key){
            return FindLeaf(key,current->firstChild->pre);
        }
        else{
            Node* result = nullptr;
            for(Node* child = current->firstChild; child; child = child->next){
                if(key >= child->key){
                    result = FindLeaf(key,child);
                    break;
                }
            }
            return result;
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

//for leaf, first is first
//for internal, first is second
void BPlusTree::InsertRecordtoLeaf(int key, Node* record,Node* leaf){
     if(key < leaf->firstChild->key){
         record->next = leaf->firstChild;
         record->pre = nullptr;
         leaf->firstChild = record;
     }
     else{
         for(Node* child = leaf->firstChild; child; child = child->next){
            if(key >= child->key){
                record->pre = child;
                record->next = child->next;
                break;
            }
         }
     }
     record->parent = leaf;
     leaf->size++;
     if(leaf->size > M){
         
     }
}

/*
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
*/
