#include <cstdio>
#include <cassert>
#include "Struct.hpp"

Node::Node(int M, bool isLeaf){
    this->Key = -1;
    this->isLeaf = isLeaf;
    this->size = 0;
    this->parent = nullptr;
    this->neighbor = nullptr;
    this->pre = nullptr;
    this->next = nullptr;
    this->M = M;
    
    key = new int[M];
    if(isLeaf){
        records = new Record*[M+1];
    }
    else{
        children = new Node*[M+1];
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
    
Node* Node::PopFrontNode(int& popKey){
    popKey = key[0];
    Node* node = children[0];
    size--;
    for(int i = 0; i < size; i++){
        if(i!=0){
            key[i-1] = key[i];
        }
        children[i] = children[i+1];
    }
    if(size){
        children[0]->pre = nullptr;
    }
    return node;
}

Node* Node::PopBackNode(int& popKey){
    popKey = key[size-2];
    size--;
    if(size){
        children[size-1]->next = nullptr;
    }
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
    children[0]->pre = nullptr;
    if(size >= 2){
        children[0]->next = children[1];
    }
    else{
        children[0]->next = nullptr;
    }
}

void Node::PushBackNode(int nkey, Node* node){
    size++;
    key[size-2] = nkey;
    children[size-1] = node;
    children[size-1]->next = nullptr;
    if(size >= 2){
        children[size-1]->pre = children[size-2];
    }
    else{
        children[size-1]->pre = nullptr;
    }
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

Node* Node::SplitLeaf(){
    assert(size == M+1);
    
    Node* newLeaf = new Node(M, true);
    size = (M+1)/2;
    for(int i = size; i < M+1; i++){
        newLeaf->PushBackRecord(records[i]);
    }
    return newLeaf;
}

void Node::PrintNode(){
    printf("SELF:\n");
    printf("size = %d\n",size);
    SimplePrint();
    printf("PARENT:\n");
    if(parent){
        parent->SimplePrint();
    }
    else printf("NULL\n");
    printf("PRE: \n");
    if(pre){
        pre->SimplePrint();
    }
    else printf("NULL\n");
    printf("NEXT: \n");
    if(next){
        next->SimplePrint();
    }
    else printf("NULL\n");
    printf("\n");
}

void Node::SimplePrint(){
    if(isLeaf){
        for(int i = 0; i < size; i++){
            printf("%d ",records[i]->key);
        }
    }
    else{
        for(int i = 0; i < size-1; i++){
            printf("%d ",key[i]);
        }
    }
    printf("\n");
}
