#include <cstdio>
#include "struct.hpp"

Node::Node(int M, bool isLeaf){
    this->isLeaf = isLeaf;
    this->size = 0;
    this->parent = nullptr;
    this->neighbor = nullptr;
    key = new int[M-1];
    if(isLeaf){
        records = new Record*[M];
    }
    else{
        children = new Node*[M];
    }
}

Node::~Node(){
    if(isLeaf){
        delete[] records;
    }
    else{
        delete[] children;
    }
}


void Node::PrintNode(){
    printf("SELF:\n");
    printf("size = %d\n",size);
    if(isLeaf){
        for(int i = 0; i < size; i++){
            printf("%d ",records[i]->key);
        }
    }
    else{
        for(int i = 0; i < size; i++){
            if(i!=0) printf("%d ",key[i-1]);
        }
    }
    printf("\nPARENT:\n");
    if(parent){
        for(int i = 0; i < parent->size-1; i++){
            printf("%d ",parent->key[i]);
        }
        printf("\n");
    }
    else printf("NULL\n");
    printf("\n");
}
