#include "tree.hpp"
#include <cstdio>
#include <cassert>

tree::tree(int M){
    this->M = M;
    root = new Node(M, true);
}

Node* tree::FindLeaf(int key, Node* current){
    if(current->isLeaf){
        return current;
    }
    else{
        if(key < current->children[1]->KEY){
            return FindLeaf(key, current->children[0]);
        }
        else{
            int index = current->size-1;
            for(int i = 1; i < current->size-1; i++){
                if(key < current->children[i+1]->KEY){
                    index = i;
                    break;
                }
            }
            return FindLeaf(key, current->children[index]);
        }
    }
}

void tree::Insert(int key, Record* value){
    Node* leaf = FindLeaf(key, root);
    InsertRecord(key, value, leaf);
}

Record* tree::FindRecord(int key){
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

void tree::InsertRecord(int key, Record* record, Node* node){
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
        Node* splitLeaf = node->SplitLeaf();
        // propagate new key up
        if(node->parent){ 
            InsertNode(splitLeaf->KEY, splitLeaf, node->parent);
        }
        else{ 
            //root overflowed, new root needed
            Node* newRoot = new Node(M, false);
            newRoot->size = 2;
            // newRoot->key[0] = splitLeaf->KEY;
            newRoot->children[0] = node;
            newRoot->children[1] = splitLeaf;
            newRoot->ResetChildrenNeighbor();
            node->parent = newRoot;
            splitLeaf->parent = newRoot;
            root = newRoot;
        }
    }
}

void tree::InsertNode(int key, Node* child, Node* node){
    child->parent = node;
    int index = node->size;
    for(int i = 1; i < node->size; i++){
        if(key < node->children[i]->KEY){
            index = i;
            break;
        }
    }
    node->size++;
    for(int i = node->size-1; i > index; i--){
        // node->key[i-1] = node->key[i-2];
        node->children[i] = node->children[i-1];
    }
    // node->key[index-1] = key;
    node->children[index] = child;
    node->ResetChildrenNeighbor();
    
    if(node->size == M+1){
        // overflow, split
        Node* splitNode = node->SplitNode();
        if(node->parent){ 
            // propagate keys up
            InsertNode(splitNode->KEY, splitNode, node->parent);
        }
        else{ 
            // root overflowed, new root needed
            Node* newRoot = new Node(M, false);
            newRoot->size = 2;
            // newRoot->key[0] = splitNode->KEY;
            newRoot->children[0] = node;
            newRoot->children[1] = splitNode;
            newRoot->ResetChildrenNeighbor();
            node->parent = newRoot;
            splitNode->parent = newRoot;
            root = newRoot;
        }
    }
}

void tree::Delete(int key){
    Node* leaf = FindLeaf(key, root);
    if(leaf){
        DeleteRecord(key, leaf);
    }
}

void tree::DeleteRecord(int key, Node* node){
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
        if(node->size < (M+1)/2){ 
            //underflow
            if(node->pre && node->pre->size > (M+1)/2){ 
                // borrow record from left neighbor
                Record* record = node->pre->PopBackRecord();
                node->PushFrontRecord(record);
                node->KEY = record->key;
            }
            else if(node->next && node->next->size > (M+1)/2){ 
                // borrow record from right neighbor
                Record* record = node->next->PopFrontRecord();
                node->next->KEY = node->next->records[0]->key;
                node->PushBackRecord(record);
            }
            else{ 
                // borrow failed, try to merge
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
                else{
                    // leaf is root, no need to adjust anything
                } 
            }
        }
    }
}

void tree::DeleteNode(Node* deleteNode, Node* node){
    int index = -1;
    for(int i = 0; i < node->size; i++){
        if(node->children[i] == deleteNode){
            index = i;
            break;
        }
    }
    node->size--;
    for(int i = index; i < node->size; i++){
        // node->key[i-1] = node->key[i];
        node->children[i] = node->children[i+1];
    }
    node->ResetChildrenNeighbor();
    if(node->size < (M+1)/2){ 
        // underflow
        if(node->pre && node->pre->size > (M+1)/2){ 
            // borrow from left
            int leftKey;
            Node* leftNode = node->pre->PopBackNode(leftKey);
            int upKey = node->KEY;
            node->children[0]->KEY = upKey;
            node->KEY = leftKey;
            node->PushFrontNode(upKey, leftNode);
        }
        else if(node->next && node->next->size > (M+1)/2){ 
            // borrow from right
            int rightKey;
            Node* rightNode = node->next->PopFrontNode(rightKey);
            int upKey = node->next->KEY;
            rightNode->KEY = upKey;
            node->PushBackNode(upKey, rightNode);
            node->next->KEY = rightKey;
        }
        else if(node->pre || node->next){ 
            // merge
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
            int upKey = deleteNode->KEY;
            for(int i = 0; i < deleteNode->size; i++){
                if(i == 0){
                    deleteNode->children[i]->KEY = upKey;
                    mergeNode->PushBackNode(upKey, deleteNode->children[i]);
                }
                else{
                    mergeNode->PushBackNode(deleteNode->children[i]->KEY, deleteNode->children[i]);
                }
            }
            delete deleteNode;
            DeleteNode(deleteNode, deleteNode->parent);
        }
        else{ 
            // current node is root
            if(node->size == 1){
                root = node->children[0];
                delete node;
            }
        }
    }
}

void tree::PrintTreeByBfs(){
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
