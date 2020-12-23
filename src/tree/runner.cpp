#include <cstdio>
#include "tree.hpp"

int main() {
    int n,m;
    scanf("%d%d",&n,&m);
    tree* bpt = new tree(m);
    for(int i = 0; i < n; i++){
        int order;
        scanf("%d",&order);
        if(order==0){
            // insert
            int key, value;
            scanf("%d%d",&key,&value);
            Record* record = new Record(key, value);
            bpt->Insert(key, record);
        }
        else{
            // delete
            int key;
            scanf("%d",&key);
            // todo
        }
        printf("step: %d\n",i);
        bpt->PrintTreeByBfs();
    }
    return 0;
}
