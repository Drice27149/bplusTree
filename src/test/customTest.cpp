#include <cstdio>
#include <cassert>
#include "../tree/tree.hpp"

int main(){
    tree* bpt = new tree(3);
    int keys[] = {3, 18, 14, 30, 32, 36, 15, 16, 12, 40, 45, 38};
    int size = 12;
    for(int i = 0; i < size; i++){
        int key = keys[i];
        Record* record = new Record(key, key);
        bpt->Insert(key, record);
        
        //
        printf("[Insert] step: %d\n",i);
        bpt->PrintTreeByBfs();
    }
    int dKeys[] = {32, 15, 16, 14, 18};
    size = 5;
    for(int i = 0; i < size; i++){
        bpt->Delete(dKeys[i]);
        
        //
        printf("[Delete] step: %d\n",i);
        bpt->PrintTreeByBfs();
    }
    return 0;
}
