#include "ordered_queue.h"

Member OrderedQueue::Top() const {
    return members_[0];
}

void OrderedQueue::ToRoot(size_t i) {
    size_t parent = (i - 1) / 2;
    while (members_[i]->sum < members_[parent]->sum) {
        std::swap(members_[parent], members_[i]);
        i = parent;
        parent = (i - 1) / 2;
    }
}

void OrderedQueue::Pull(size_t i) {
    size_t len = members_.size();
    while (1 + 2 * i < len) {
        size_t lef = 2 * i + 1;
        size_t rig = 2 * i + 2;
        if (rig == len) {
            if (members_[i]->sum > members_[lef]->sum) {
                std::swap(members_[i], members_[lef]);
            }
            break;
        }
        size_t minimal = lef;
        if (members_[rig]->sum < members_[lef]->sum) {
            minimal = rig;
        }
        if (members_[i]->sum <= members_[minimal]->sum) {
            break;
        }
        std::swap(members_[i], members_[minimal]);
        i = minimal;
    }
}

size_t OrderedQueue::Size() const {
    return members_.size();
}

void OrderedQueue::Push(Member member) {
    members_.push_back(member);
}

void OrderedQueue::Pop() {
    members_[0] = members_.back();
    members_.pop_back();
    Pull(0);
}