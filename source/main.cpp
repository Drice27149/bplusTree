#include <cstdio>
#include "BPlusTree.hpp"

int main(){
    BPlusTree* bpt = new BPlusTree(2);
    int keys[] = {1,2,3,4,5,6};
    int size = 4;
    for(int i = 0; i < size; i++){
        printf("step = %d\n",i);
        int key = keys[i];
        Record* record = new Record(key, key); 
        //if(i==3) bpt->debug = true;
        //else bpt->debug = false;
        bpt->Insert(key, record);
    }
    bpt->PrintTreeByBfs();
    return 0;
}
