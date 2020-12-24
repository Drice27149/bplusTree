#include <cassert>
#include "../tree/struct.hpp"

void TestPushBackRecord(){
    int key[] = {1,2,3,4,5,6};
    int value[] = {1,2,3,5,8,11};
    Record* keep[6];
    Node* node = new Node(6, true);
    for(int i = 0; i < 6; i++){
        Record* record = new Record(key[i], value[i]);
        keep[i] = record;
        node->PushBackRecord(record);
    }
    
    assert(node->size == 6);
    for(int i = 0; i < 6; i++){
        assert(node->records[i]->key == key[i]);
        assert(node->records[i]->value == value[i]);
        assert(node->records[i] == keep[i]);
    }
    
    // finished
    for(int i = 0; i < 6; i++){
        delete keep[i];
    }
    delete node;
}

void TestPushFrontRecord(){
    int key[] = {1,2,3,4,5,6};
    int value[] = {1,2,3,5,8,11};
    Record* keep[6];
    Node* node = new Node(6, true);
    for(int i = 0; i < 6; i++){
        Record* record = new Record(key[i], value[i]);
        keep[i] = record;
        node->PushFrontRecord(record);
    }
    
    assert(node->size == 6);
    for(int i = 0; i < 6; i++){
        int pos = 6-i-1;
        assert(node->records[i]->key == key[pos]);
        assert(node->records[i]->value == value[pos]);
        assert(node->records[i] == keep[pos]);
    }
    
    // finished
    for(int i = 0; i < 6; i++){
        delete keep[i];
    }
    delete node;
}

int main() {
    TestPushBackRecord();
    TestPushFrontRecord();
    
    return 0;
}
