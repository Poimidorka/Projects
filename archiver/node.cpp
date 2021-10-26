#include "node.h"

Node::Node(size_t sum) {
    l = r = nullptr;
    parent = nullptr;
    this->sum = sum;
};

void Node::InlineRemoveNode(Node *head) {
    std::vector<Node*> stack;
    std::vector<size_t> count;
    stack.push_back(head);
    count.push_back(0);
    while (!stack.empty()) {
        Node* element = stack.back();
        size_t& counter = count.back();
        if (element == nullptr) {
            stack.pop_back();
            count.pop_back();
            continue;
        }
        counter++;
        if (counter == 1) {
            stack.push_back(element->l);
            count.push_back(0);
        }
        else if (counter == 2) {
            stack.push_back(element->r);
            count.push_back(0);
        }
        else if (counter == 3){
            delete element;
            stack.pop_back();
            count.pop_back();
        }
    }
}
