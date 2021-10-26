#pragma once

#include <vector>
#include "node.h"

using Member = Node*;

class OrderedQueue {
public:
    size_t Size() const;
    Member Top() const;
    void Push(Member member);
    void Pop();

private:
    void ToRoot(size_t i);
    void Pull(size_t i);
    std::vector<Member> members_;
};