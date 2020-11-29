#include "BPlusTree.h"

int main(){
    //basic insertion test
    BPlusTree* bpT = new BPlusTree(3);
    for(int i = 1; i <= 4; i++){
        printf("i = %d\n",i);
        int key = i;
        Node* newNode = new Node();
        newNode->key = key;
        bpT->Insert(key);
    }
    
    return 0;
}
