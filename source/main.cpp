#include <cstdio>
#include "BPlusTree.hpp"

int main(){
    BPlusTree* bpt = new BPlusTree(3);
    int keys[] = {3, 18, 14, 30, 32, 36, 15, 16, 12, 40, 45, 38};
    int size = 12;
    for(int i = 0; i < size; i++){
        printf("step = %d\n",i);
        int key = keys[i];
        Record* record = new Record(key, key); 
        //if(i==3) bpt->debug = true;
        //else bpt->debug = false;
        bpt->Insert(key, record);
    }
    bpt->PrintTreeByBfs();
    
    int deleteKeys[] = {32, 15, 16, 14, 18};
    size = 5;
    for(int i = 0; i < size; i++){
        bpt->Delete(deleteKeys[i]);
    }
    bpt->PrintTreeByBfs();
    
    return 0;
}
