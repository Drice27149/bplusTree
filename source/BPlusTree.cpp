#include "BPlusTree.hpp"
#include <cstdio>
#include <cassert>

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
            newRoot->ResetChildrenNeighbor();
            parent->parent = newRoot;
            splitLeaf->parent = newRoot;
            root = newRoot;
        }
    }
}

void BPlusTree::InsertNode(int key, Node* child, Node* node){
    int index = node->size;
    for(int i = 1; i < node->size; i++){
        if(key < node->key[i-1]){
            index = i;
            break;
        }
    }
    if(node->size != M){
        node->size++;
        for(int i = node->size; i > index; i--){
            node->key[i-1] = node->key[i-2];
            node->children[i] = node->children[i-1];
        }
        node->key[index-1] = key;
        node->children[index] = child;
        if(index){
            child->pre = node->children[index-1];
            node->children[index-1]->next = child;
        }
        if(index+1 < node->size){
            child->next = node->children[index+1];
            node->children[index+1]->pre = child;
        }
    }
    else{ // overflow, split
        Node* tempNode[M+1];
        int tempKey[M];
        int ptr = 0;
        for(int i = 0; i < node->size; i++){
            if(i==index){
                tempKey[ptr-1] = key;
                tempNode[ptr] = child;
                ptr++;
            }
            if(i!=0){
                tempKey[ptr-1] = node->key[i-1];
            }
            tempNode[ptr] = node->children[i];
            ptr++;
        }
        if(index==node->size){
            tempKey[ptr-1] = key;
            tempNode[ptr] = child;
        }
        
        //lelf part of the split node
        node->size = (M+1)/2;
        for(int i = 0; i < node->size; i++){
            if(i!=0){
                node->key[i-1] = tempKey[i-1];
            }
            node->children[i] = tempNode[i];
            //reset parent
            node->children[i]->parent = node;
        }
        node->ResetChildrenNeighbor();
        
        //right part of the split node
        Node* splitNode = new Node(M, node->isLeaf);
        splitNode->size = (M+1) - node->size;
        splitNode->parent = node->parent;
        for(int i = 0; i < splitNode->size; i++){
            int offset = i+node->size;
            if(i!=0){
                splitNode->key[i-1] = tempKey[offset-1];
            }
            splitNode->children[i] = tempNode[offset];
            //reset parent            
            splitNode->children[i]->parent = splitNode;
        }
        splitNode->ResetChildrenNeighbor();
        
        int pushUpKey = tempKey[node->size-1];
        if(node->parent){ // propagate keys up
            InsertNode(pushUpKey, splitNode, node->parent);
        }
        else{ // root overflowed, new root needed
            Node* newRoot = new Node(M, false);
            newRoot->size = 2;
            newRoot->key[0] = pushUpKey;
            newRoot->children[0] = node;
            newRoot->children[1] = splitNode;
            newRoot->ResetChildrenNeighbor();
            node->parent = newRoot;
            splitNode->parent = newRoot;
            root = newRoot;
        }
    }
}

void BPlusTree::Delete(int key){
    Node* leaf = FindLeaf(key, root);
    if(leaf){
        DeleteRecord(key, leaf);
    }
}

void BPlusTree::DeleteRecord(int key, Node* node){
    int index = -1;
    for(int i = 0; i < node->size; i++){
        if(node->records[i]->key == key){
            index = i;
            break;
        }
    }
    if(index != -1){
        delete node->records[index];
        node->size--;
        for(int i = index; i < node->size; i++){
            node->records[i] = node->records[i+1];
        }
        if(node->size < (M+1)/2){ //underflow
            if(node->pre && node->pre->size > (M+1)/2){ // borrow record from left neighbor
                Record* record = node->pre->PopBackRecord();
                node->PushFrontRecord(record);
                UpdateKey(record->key, node->parent);
            }
            else if(node->next && node->next->size > (M+1)/2){ // borrow record from right neighbor
                Record* record = node->next->PopFrontRecord();
                node->PushBackRecord(record);
                UpdateKey(record->key, node->parent);
            }
            else{ // borrow failed, try to merge
                if(node->pre || node->next){
                    Node* mergeNode = nullptr;
                    Node* deleteNode = nullptr;
                    if(node->pre){
                        Node* mergeNode = node->pre;
                        Node* deleteNode = node;
                    }
                    else{
                        Node* mergeNode = node;
                        Node* deleteNode = node->next;
                    }
                    for(int i = 0; i < deleteNode->size; i++){
                        mergeNode->PushBackRecord(deleteNode->records[i]);
                    }
                    delete deleteNode;
                    DeleteNode(deleteNode, mergeNode->parent);
                }
                else{} // leaf is root, no need to adjust anything
            }
        }
    }
}

void BPlusTree::UpdateKey(int newKey, Node* node){
    for(int i = 0; i < node->size-1; i++){
        bool left = i==0 || newKey > node->key[i-1];
        bool right = i==node->size-2 || newKey < node->key[i+1];
        if(left && right){
            node->key[i] = newKey;
            break;
        }
    }
}

void BPlusTree::DeleteNode(Node* deleteNode, Node* node){
    int index = -1;
    for(int i = 0; i < node->size; i++){
        if(node->children[i] == deleteNode){
            index = i;
            break;
        }
    }
    node->size--;
    for(int i = index; i < node->size; i++){
        node->key[i-1] = node->key[i];
        node->children[i] = node->children[i+1];
    }
    node->ResetChildrenNeighbor();
    if(node->size < (M+1)/2){ // underflow
        if(node->pre && node->pre->size > (M+1)/2){ // borrow from left
            int leftKey;
            Node* leftNode = node->pre->PopBackNode(leftKey);
            int upKey = GetKeyByChildNode(node, node->parent);
            node->PushFrontNode(upKey, leftNode);
            UpdateKey(leftKey, node->parent);
        }
        else if(node->next && node->next->size > (M+1)/2){ // borrow from right
            int rightKey;
            Node* rightNode = node->next->PopFrontNode(rightKey);
            int upKey = GetKeyByChildNode(node->next, node->parent);
            node->PushBackNode(upKey, rightNode);
            UpdateKey(rightKey, node->parent);
        }
        else if(node->pre || node->next){ // merge
            Node* mergeNode = nullptr;
            Node* deleteNode = nullptr;
            if(node->pre){
                mergeNode = node->pre;
                deleteNode = node;
            }
            else{
                mergeNode = node;
                deleteNode = node->next;
            }
            int upKey = GetKeyByChildNode(deleteNode, node->parent);
            for(int i = 0; i < deleteNode->size; i++){
                if(i == 0){
                    mergeNode->PushBackNode(upKey, deleteNode->children[i]);
                }
                else{
                    mergeNode->PushBackNode(deleteNode->key[i-1], deleteNode->children[i]);
                }
            }
            delete deleteNode;
            DeleteNode(deleteNode, deleteNode->parent);
        }
        else{ // current node is root
            if(node->size == 1){
                root = node->children[0];
                delete node;
            }
        }
    }
}

int BPlusTree::GetKeyByChildNode(Node* childNode, Node* node){
    int result = -1;
    for(int i = 0; i < node->size-1; i++){
        if(node->children[i+1] == childNode){
            result = node->key[i];
            break;
        }
    }
    return result;
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
