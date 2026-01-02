#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include "../MemoryMenagment/memory_menagment.h"

struct Node {
    void* next;
    int value;
};

struct DataChunk {
    double values[4];
    int id;
};


class GCHeapTest : public ::testing::Test {
protected:
    std::unique_ptr<GCHeap> heap;

    TypeInfo nodeType;
    uint32_t nodeOffsets[1] = { 0 }; 

    TypeInfo dataType;

    void SetUp() override {
        heap = std::make_unique<GCHeap>(128 * 1024);
        nodeType.size = sizeof(Node);
        nodeType.numPointers = 1;
        nodeType.pointerOffsets = nodeOffsets;
        dataType.size = sizeof(DataChunk);
        dataType.numPointers = 0;
        dataType.pointerOffsets = nullptr;
    }
};

TEST_F(GCHeapTest, BasicRelocation) {
    Node* n = (Node*)heap->allocate(sizeof(Node), &nodeType);
    n->value = 100;
    n->next = nullptr;

    void* oldAddr = n;
    heap->registerRoot((void**)&n);

    heap->collect();

    EXPECT_NE(oldAddr, (void*)n);
    EXPECT_EQ(n->value, 100);    
}

TEST_F(GCHeapTest, InternalPointerPatching) {
    Node* n1 = (Node*)heap->allocate(sizeof(Node), &nodeType);
    Node* n2 = (Node*)heap->allocate(sizeof(Node), &nodeType);

    n1->value = 1;
    n2->value = 2;
    n1->next = n2;

    heap->registerRoot((void**)&n1);
    heap->registerRoot((void**)&n2);

    void* oldN2Addr = n2;

    heap->collect();

    EXPECT_NE(oldN2Addr, (void*)n2);
    EXPECT_EQ(n1->next, (void*)n2);
    EXPECT_EQ(((Node*)n1->next)->value, 2);
}

TEST_F(GCHeapTest, CircularReference) {
    Node* n1 = (Node*)heap->allocate(sizeof(Node), &nodeType);
    Node* n2 = (Node*)heap->allocate(sizeof(Node), &nodeType);

    n1->next = n2;
    n2->next = n1;

    heap->registerRoot((void**)&n1);
    heap->registerRoot((void**)&n2);

    EXPECT_NO_THROW(heap->collect());

    EXPECT_EQ(n1->next, (void*)n2);
    EXPECT_EQ(n2->next, (void*)n1);
}

TEST_F(GCHeapTest, DeepObjectChain) {
    const int CHAIN_SIZE = 50;
    Node* headNode = (Node*)heap->allocate(sizeof(Node), &nodeType);
    heap->registerRoot((void**)&headNode);

    Node* current = headNode;
    for (int i = 0; i < CHAIN_SIZE; i++) {
        Node* nextNode = (Node*)heap->allocate(sizeof(Node), &nodeType);
        nextNode->value = i;
        nextNode->next = nullptr;
        current->next = nextNode;
        current = nextNode;
    }

    heap->collect();
    Node* checker = headNode;
    for (int i = 0; i < CHAIN_SIZE; i++) {
        checker = (Node*)checker->next;
        ASSERT_NE(checker, nullptr);
        EXPECT_EQ(checker->value, i);
    }
}

TEST_F(GCHeapTest, MemoryReclamation) {
    void* trash = heap->allocate(sizeof(DataChunk), &dataType);
    heap->collect();
    void* fresh = heap->allocate(sizeof(DataChunk), &dataType);
    EXPECT_NE(trash, fresh);
}

TEST_F(GCHeapTest, AddressAlignment) {
    heap->allocate(1, &dataType);
    void* p2 = heap->allocate(1, &dataType);
    void* p3 = heap->allocate(1, &dataType);

    EXPECT_EQ((uintptr_t)p2 % 8, 0);
    EXPECT_EQ((uintptr_t)p3 % 8, 0);
}

TEST_F(GCHeapTest, AutomaticTriggerRecoversSpace) {

    TypeInfo hugeType = { 100 * 1024, 0, nullptr };
    heap->allocate(100 * 1024, &hugeType); 

    EXPECT_NO_THROW({
        void* newObj = heap->allocate(100 * 1024, &hugeType);
        EXPECT_NE(newObj, nullptr);
    });
}
TEST_F(GCHeapTest, StressTest) {
    Node* root = (Node*)heap->allocate(sizeof(Node), &nodeType);
    heap->registerRoot((void**)&root);
    root->value = 0;

    for (int i = 0; i < 100; i++) {
        root->value++;
        heap->collect();
    }

    EXPECT_EQ(root->value, 100);
}