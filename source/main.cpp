#include <cstdio>
#include <cassert>
#include "BPlusTree.hpp"

int keys[] = {1, 2, 3, 4, 5, 6};
Record* keep[55];

int main(){
    BPlusTree* bpt = new BPlusTree(3);
    int keys[] = {3, 2, 3, 4, 5, 36, 15, 16, 12, 40, 45, 38};
    int size = 12;
    for(int i = 0; i < size; i++){
        int key = keys[i];
        Record* record = new Record(key, key);
        bpt->Insert(key, record);
    }
    int dKeys[] = {32, 15, 14, 16, 18};
    size = 5;
    for(int i = 0; i < size; i++){
        bpt->Delete(dKeys[i]);
    }
    bpt->PrintTreeByBfs();
    return 0;
}
