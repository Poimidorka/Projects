#pragma once
#include <cstddef>
#include <vector>

struct Node {
    Node* l;
    Node* r;
    Node* parent;
    size_t sum = 0;
    Node (size_t sum);
    void InlineRemoveNode(Node *head);
};
