#include "BPlusTree.hpp"
#include <cstdio>

BPlusTree::BPlusTree(int M){
    this->M = M;
    root = new Node(M, true);
}

Node* BPlusTree::FindLeaf(int key, Node* current){
    if(current->isLeaf){
        return current;
    }
    else{
        if(key < current->key[0]){
            return FindLeaf(key, current->children[0]);
        }
        else{
            int index = current->size-1;
            for(int i = 1; i < current->size-1; i++){
                if(key < current->key[i]){
                    index = i;
                    break;
                }
            }
            return FindLeaf(key, current->children[index]);
        }
    }
}

void BPlusTree::Insert(int key, Record* value){
    Node* leaf = FindLeaf(key, root);
    InsertRecord(key, value, leaf);
}

void BPlusTree::InsertRecord(int key, Record* record, Node* parent){
    int index = parent->size;
    for(int i = 0; i < parent->size; i++){
        if(key < parent->records[i]->key){
            index = i;
            break;
        }
    }
    if(parent->size != M){
        parent->size++;
        for(int i = parent->size-1; i > index; i--){
            parent->records[i] = parent->records[i-1];
        }
        parent->records[index] = record;
    }
    else{
        //overflow, split
        Record* tempRecord[M+1];
        int ptr = 0;
        for(int i = 0; i < parent->size; i++){
            if(i==index){
                tempRecord[ptr++] = record;
            }
            tempRecord[ptr++] = parent->records[i];
        }
        if(index==parent->size) tempRecord[ptr++] = record;
        //left part of the split node
        parent->size = (M+1)/2;
        for(int i = 0; i < parent->size; i++){
            parent->records[i] = tempRecord[i];
        }
        //right part of the split node
        Node* splitLeaf = new Node(M, true);
        splitLeaf->size = (M+1) - parent->size;
        splitLeaf->parent = parent->parent;
        for(int i = 0; i < splitLeaf->size; i++){
            int offset = i+parent->size;
            splitLeaf->records[i] = tempRecord[offset];
        }
        int pushUpKey = splitLeaf->records[0]->key;
        if(parent->parent){ //propagate keys up
            InsertNode(pushUpKey, splitLeaf, parent->parent);
        }
        else{ //root overflowed, new root needed
            Node* newRoot = new Node(M, false);
            newRoot->size = 2;
            newRoot->key[0] = pushUpKey;
            newRoot->children[0] = parent;
            newRoot->children[1] = splitLeaf;
            parent->parent = newRoot;
            splitLeaf->parent = newRoot;
            root = newRoot;
        }
    }
}

void BPlusTree::InsertNode(int key, Node* child, Node* parent){
    int index = parent->size;
    for(int i = 1; i < parent->size; i++){
        if(key < parent->key[i-1]){
            index = i;
            break;
        }
    }
    if(parent->size != M){
        parent->size++;
        for(int i = parent->size; i > index; i--){
            parent->key[i-1] = parent->key[i-2];
            parent->children[i] = parent->children[i-1];
        }
        parent->key[index-1] = key;
        parent->children[index] = child;
    }
    else{ // overflow, split
        Node* tempNode[M+1];
        int tempKey[M];
        int ptr = 0;
        for(int i = 0; i < parent->size; i++){
            if(i==index){
                tempKey[ptr-1] = key;
                tempNode[ptr] = child;
                ptr++;
            }
            if(i!=0){
                tempKey[ptr-1] = parent->key[i-1];
            }
            tempNode[ptr] = parent->children[i];
            ptr++;
        }
        if(index==parent->size){
            tempKey[ptr-1] = key;
            tempNode[ptr] = child;
        }
        //lelf part of the split node
        parent->size = (M+1)/2;
        for(int i = 0; i < parent->size; i++){
            if(i!=0){
                parent->key[i-1] = tempKey[i-1];
            }
            parent->children[i] = tempNode[i];
            //reparent
            tempNode[i]->parent = parent;
        }
        //right part of the split node, **key is the orginal middle key**
        Node* splitNode = new Node(M, parent->isLeaf);
        splitNode->size = (M+1) - parent->size;
        splitNode->parent = parent->parent;
        for(int i = 0; i < splitNode->size; i++){
            int offset = i+parent->size;
            if(i!=0){
                splitNode->key[i-1] = tempKey[offset-1];
            }
            splitNode->children[i] = tempNode[offset];
            //reparent
            tempNode[offset]->parent = splitNode;
        }
        int pushUpKey = tempKey[parent->size-1];
        if(parent->parent){ // propagate keys up
            InsertNode(pushUpKey, splitNode, parent->parent);
        }
        else{ // root overflowed, new root needed
            Node* newRoot = new Node(M, false);
            newRoot->size = 2;
            newRoot->key[0] = pushUpKey;
            newRoot->children[0] = parent;
            newRoot->children[1] = splitNode;
            parent->parent = newRoot;
            splitNode->parent = newRoot;
            root = newRoot;
        }
    }
}

void BPlusTree::PrintTreeByBfs(){
    Node* queue[1005];
    int head = 0, tail = -1;
    queue[++tail] = root;
    while(head <= tail){
        Node* current = queue[head++];
        current->PrintNode();
        if(!current->isLeaf){
            for(int i = 0; i < current->size; i++){
                queue[++tail] = current->children[i];
            }
        }
    }
}
