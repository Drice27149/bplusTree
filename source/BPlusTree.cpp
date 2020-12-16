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
        if(key < current->key[0] /*current->children[1]->KEY*/){
            return FindLeaf(key, current->children[0]);
        }
        else{
            int index = current->size-1;
            for(int i = 1; i < current->size-1; i++){
                if(key < current->key[i]  /*current->children[i+1]->KEY*/){
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

Record* BPlusTree::FindRecord(int key){
    Node* leaf = FindLeaf(key, root);
    Record* result = nullptr;
    if(leaf){
        for(int i = 0; i < leaf->size; i++){
            if(key == leaf->records[i]->key){
                result = leaf->records[i];
                break;
            }
        }
    }
    return result;
}

void BPlusTree::InsertRecord(int key, Record* record, Node* node){
    int index = node->size;
    for(int i = 0; i < node->size; i++){
        if(key < node->records[i]->key){
            index = i;
            break;
        }
    }
    node->size++;
    for(int i = node->size-1; i > index; i--){
        node->records[i] = node->records[i-1];
    }
    node->records[index] = record;

    if(node->size == M+1){
        // overflow, split
        int pushUpKey = node->GetPushUpKey();
        Node* splitLeaf = node->SplitLeaf();
        splitLeaf->KEY = pushUpKey;
        // propagate new key up
        // int pushUpKey = splitLeaf->records[0]->key;
        if(node->parent){ 
            InsertNode(pushUpKey, splitLeaf, node->parent);
        }
        else{ //root overflowed, new root needed
            Node* newRoot = new Node(M, false);
            newRoot->size = 2;
            newRoot->key[0] = pushUpKey;
            newRoot->children[0] = node;
            newRoot->children[1] = splitLeaf;
            newRoot->ResetChildrenNeighbor();
            node->parent = newRoot;
            splitLeaf->parent = newRoot;
            root = newRoot;
        }
    }
}

void BPlusTree::InsertNode(int key, Node* child, Node* node){
    child->parent = node;
    int index = node->size;
    for(int i = 1; i < node->size; i++){
        if(key < node->key[i-1] /*node->children[i]->KEY*/){
            index = i;
            break;
        }
    }
    node->size++;
    for(int i = node->size-1; i > index; i--){
        node->key[i-1] = node->key[i-2];
        node->children[i] = node->children[i-1];
    }
    node->key[index-1] = key;
    node->children[index] = child;
    node->ResetChildrenNeighbor();
    
    if(node->size == M+1){
        // overflow, split
        int pushUpKey = node->GetPushUpKey();
        Node* splitNode = node->SplitNode();
        splitNode->KEY = pushUpKey;

        if(node->parent){ 
            // propagate keys up
            InsertNode(pushUpKey, splitNode, node->parent);
        }
        else{ 
            // root overflowed, new root needed
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
                // add
                Record* record = node->pre->PopBackRecord();
                node->PushFrontRecord(record);
                node->KEY = record->key;
                // 
                UpdateKey(record->key, node->parent);
            }
            else if(node->next && node->next->size > (M+1)/2){ // borrow record from right neighbor
                Record* record = node->next->PopFrontRecord();
                // add
                node->next->KEY = node->records[0]->key;
                node->PushBackRecord(record);
                //
                UpdateKey(record->key, node->parent);
            }
            else{ // borrow failed, try to merge
                if(node->pre || node->next){
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
            // add, set inside pushfront
            node->PushFrontNode(upKey, leftNode);
            node->KEY = leftKey;
            UpdateKey(leftKey, node->parent);
        }
        else if(node->next && node->next->size > (M+1)/2){ // borrow from right
            int rightKey;
            Node* rightNode = node->next->PopFrontNode(rightKey);
            int upKey = GetKeyByChildNode(node->next, node->parent);
            // add, set inside pushback
            node->PushBackNode(upKey, rightNode);
            node->KEY = rightKey;
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
