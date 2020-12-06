#include <cstdio>
#include "Struct.hpp"

Node::Node(int M, bool isLeaf){
    this->isLeaf = isLeaf;
    this->size = 0;
    this->parent = nullptr;
    this->neighbor = nullptr;
    this->pre = nullptr;
    this->next = nullptr;
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

Record* Node::PopFrontRecord(){
    Record* record = records[0];
    size--;
    for(int i = 0; i < size; i++){
        records[i] = records[i+1];
    }
    return record;
}

Record* Node::PopBackRecord(){
    size--;
    return records[size];
}

void Node::PushBackRecord(Record* record){
    records[size++] = record;
}

void Node::PushFrontRecord(Record* record){
    size++;
    for(int i = size-1; i > 0; i--){
        records[i] = records[i-1];
    }
    records[0] = record;
}
    
Node* Node::PopFrontNode(){
    Node* node = children[0];
    size--;
    for(int i = 0; i < size; i++){
        if(i!=0){
            key[i-1] = key[i];
        }
        children[i] = children[i+1];
    }
    return node;
}

Node* Node::PopBackNode(){
    size--;
    return children[size];
}

void Node::PushFrontNode(int nkey, Node* node){
    size++;
    for(int i = size-1; i > 0; i--){
        if(i>=2){
            key[i-1] = key[i-2];
        }
        children[i] = children[i-1];
    }
    children[0] = node;
    key[0] = nkey;
}

void Node::PushBackNode(int nkey, Node* node){
    size++;
    key[size-2] = nkey;
    children[size-1] = node;
}


void Node::ResetChildrenNeighbor(){
    for(int i = 0; i < size; i++){
        children[i]->pre = nullptr;
        children[i]->next = nullptr;
        if(i){
            children[i]->pre = children[i-1];
            children[i-1]->next = children[i];
        }
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
