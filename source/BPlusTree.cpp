#include "BPlusTree.h"

int main(){
    //test insert 
    /*int insertSeq[] = {3, 18, 14, 30, 32, 36, 15, 16, 12, 40, 45, 38};
    int insertLen = 4;
    int testM = 3;
    BPlusTree* bpT = new BPlusTree(testM);
    for(int i = 0; i < insertLen; i++){
        bpT->Insert(insertSeq[i]);
    }
    bpT->PrintTree();*/
    Node** first = new Node* [15];
    for(int i = 0; i < 20; i++){
        if(first[i]!=nullptr){
            printf("i = %d\n",i);
        }
    }
    return 0;
}
