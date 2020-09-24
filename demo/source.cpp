#include<cstdio>
const int M = 3;
//ptr[i]->[key[i],key[i+1])
//ptr[0]->(-inf,key[1])
//ptr[size]->[key[size],inf)
//key index: [1,M-1], max count: normally it is M-1, but could be M after insert & before maintain
//ptr index: [0,size], max count: related to key size
//size: number of keys
//leaf: needed?
//on delete: pointer to brother is needed
struct Node{
    Node* parent; 
    int key[M+1];
    Node* ptr[M+1];
    int size;
    bool leaf;
};

void addEntryToLeaf(Node* current,int newKey){
    int findPos = 1;
    for(int i = 1; i <= current->size; i++){
        if(newKey>current->key[i]){
            findPos = i+1;
            break;
        }
    }
    current->size += 1;
    for(int i = current->size; i >= findPos+1; i--){
        current->key[i] = current->key[i-1];
        current->ptr[i] = current->ptr[i-1];
    }
    //for leaf, ptr==null?
    current->key[findPos] = newKey;
    current->ptr[findPos] = NULL;
}

void splitInternal(Node* current,int& newKey,Node*& left,Node*& right){
    int mid = (M+1)/2;
    newKey = current->key[mid];
    //this is split right part
    Node* newNode = new Node();
    newNode->size = 0;
    newNode->leaf = false;
    for(int i = mid+1; i <= current->size; i++) addEntryToLeaf(newNode,current->key[i]);
    newNode->ptr[0] = current->ptr[mid];
    //current node as split left part
    current->size = mid-1;
    if(current->parent==NULL){
        Node* newRoot = new Node();
        newRoot->size = 0;
        newRoot->leaf = false;
        current->parent = newNode->parent = newRoot;
    }
    else newNode->parent = current->parent;
    left = current, right = newNode;
}

void addEntryToInternal(Node* current,int newKey,Node* left,Node* right){
    int findPos = 1;
    for(int i = 1; i <= current->size; i++){
        if(newKey>current->key[i]){
            findPos = i+1;
            break;
        }
    }
    current->size += 1;
    for(int i = current->size; i >= findPos+1; i--){
        current->key[i] = current->key[i-1];
        current->ptr[i] = current->ptr[i-1];
    }
    current->key[findPos] = newKey;
    current->ptr[findPos] = right;
    current->ptr[findPos-1] = left;
    if(current->size==M){
        splitInternal(current,newKey,left,right);
        addEntryToInternal(current->parent,newKey,left,right);
    }
}

void splitLeaf(Node* current){
    int mid = (M+1)/2;
    int midKey = current->key[mid];
    //this is split right part
    Node* newNode = new Node();
    newNode->size = 0;
    newNode->leaf = true;
    for(int i = mid; i <= current->size; i++) addEntryToLeaf(newNode,current->key[i]);
    //current node as split left part
    current->size = mid-1;
    if(current->parent==NULL){
        Node* newRoot = new Node();
        newRoot->size = 0;
        newRoot->leaf = false;
        current->parent = newNode->parent = newRoot;
    }
    else newNode->parent = current->parent;
    addEntryToInternal(current->parent,midKey,current,newNode);
}

void NodeInsert(Node* current,int newKey){
    if(current->leaf==true){
        addEntryToLeaf(current,newKey);
        if(current->size==M) splitLeaf(current);
    }
    else{
        if(newKey<current->key[1]) NodeInsert(current->ptr[0],newKey);
        else{
            for(int i = 1; i <= current->size; i++){
                if(newKey>=current->key[i]){
                    NodeInsert(current->ptr[i],newKey);
                    break;
                }
            }
        }
    }
}

int main(){
    Node* root = new Node();
    root->size = 0;
    root->leaf = true;
    int n;
    scanf("%d",&n);
    for(int i = 1; i <= n; i++){
        int u;
        scanf("%d",&u);
        NodeInsert(root,u);
    }
    return 0;
}
