#include <cstdio>
#include <cassert>
#include "struct.hpp"

Node::Node(int M, bool isLeaf){
    this->KEY = -1;
    this->isLeaf = isLeaf;
    this->size = 0;
    this->parent = nullptr;
    this->pre = nullptr;
    this->next = nullptr;
    this->M = M;
    
    // key = nullptr;
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
    popKey = children[1]->KEY;
    Node* node = children[0];
    size--;
    for(int i = 0; i < size; i++){
        children[i] = children[i+1];
    }
    if(size){
        children[0]->pre = nullptr;
    }
    return node;
}

Node* Node::PopBackNode(int& popKey){
    popKey = children[size-1]->KEY;
    size--;
    if(size){
        children[size-1]->next = nullptr;
    }
    return children[size];
}

void Node::PushFrontNode(int nkey, Node* node){ 
    size++;
    for(int i = size-1; i > 0; i--){
        children[i] = children[i-1];
    }
    children[0] = node;
    children[0]->pre = nullptr;
    // reset parent
    node->parent = this;
    // reset neighbor
    if(size >= 2){
        children[0]->next = children[1];
        children[1]->pre = children[0];
    }
    else{
        children[0]->next = nullptr;
    }
}

void Node::PushBackNode(int nkey, Node* node){
    size++;
    children[size-1] = node;
    children[size-1]->next = nullptr;
    // reset parent
    node->parent = this;
    // reset neighbor
    if(size >= 2){
        children[size-2]->next = node;
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
    // new leaf's key is given on split
    
    Node* newLeaf = new Node(M, true);
    size = (M+1)/2;
    for(int i = size; i < M+1; i++){
        if(i==size){
            newLeaf->KEY = records[i]->key;
        }
        newLeaf->PushBackRecord(records[i]);
    }
    return newLeaf;
}

Node* Node::SplitNode(){
    assert(size == M+1);
    // new node's key is given on split
    
    Node* newNode = new Node(M, isLeaf);
    size = (M+1)/2;
    // reset neighbor
    children[size-1]->next = nullptr;
    children[size]->pre = nullptr;
    for(int i = size; i < M+1; i++){
        if(i==size){
            newNode->KEY = children[i]->KEY;
        }
        newNode->PushBackNode(children[i]->KEY, children[i]);
    }
    return newNode;
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
            printf("%d ",children[i+1]->KEY);
        }
    }
    printf("\n");
}
