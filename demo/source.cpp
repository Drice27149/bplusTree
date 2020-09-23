#include<cstdio>
const int M = 7;
//ptr[i]->[key[i],key[i+1])
//ptr[0]->(-inf,key[1])
//ptr[size]->[key[size],inf)
//key index: [1,M-1], max count: normally it is M-1, but could be M after insert & before maintain
//ptr index: [0,size], max count: related to key size
//size: number of keys
struct Node{
    Node* parent; 
    int key[M+1];
    Node* ptr[M+1];
    int size;
};

//test push
int main(){
    
    
    
    return 0;
}
