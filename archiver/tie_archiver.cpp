#include "tie_archiver.h"
#include "ordered_queue.h"

void TieArchiver::InlineClearTie() {
    std::vector<Node*> stack;
    std::vector<size_t> count;
    stack.push_back(head_);
    count.push_back(0);
    while (!stack.empty()) {
        Node* element = stack.back();
        size_t& counter = count.back();
        if (element == nullptr) {
            stack.pop_back();
            count.pop_back();
            continue;
        }
        ++counter;
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
    head_ = nullptr;
    current_ = nullptr;
}

TieArchiver::TieArchiver() : head_(nullptr), current_(nullptr) {
}

void TieArchiver::GetRootPath(Node* current, size_t& len) const {
    while (current->parent) {
        ++len;
        current = current->parent;
    }
}

bool TieArchiver::GetNext(bool c)  {
    if (!c) {
        current_ = current_->l;
    }
    else {
        current_ = current_->r;
    }
    bool f = (current_->l == current_->r && current_->r == nullptr);
    if (f) {
        current_ = head_;
    }
    return f;
}

void TieArchiver::Build(CellList& cells) {
    if (head_) {
        InlineClearTie();
    }
    head_ = new Node(0);
    for (auto el : cells) {
        BigCode a = el.code;
        current_ = head_;
        for (bool c : a) {
            if (c) {
                if (current_->r == nullptr) {
                    current_->r = new Node(0);
                }
                current_ = current_->r;
            }
            else {
                if (current_->l == nullptr) {
                    current_->l = new Node(0);
                }
                current_ = current_->l;
            }
        }
        current_ = head_;
    }
}

void TieArchiver::GetCanon(CellList& cell_list) const {
    if (cell_list.empty()) {
        throw std::runtime_error("bad format");
    }
    BigCode current(cell_list[0].len);
    for (size_t i = 0; i < cell_list.size(); ++i) {
        cell_list[i].code = current;
        if (i + 1 < cell_list.size()) {
            size_t ind = current.size() - 1;
            while (current[ind] == 1) {
                current[ind] = 0;
                --ind;
            }
            current[ind] = 1;
            for (size_t j = 0; j < cell_list[i + 1].len - cell_list[i].len; ++j) {
                current.push_back(0);
            }
        }
    }
}

void TieArchiver::CodesByCnt(CellList &cell_list) {
    if (cell_list.size() == 1) {
        cell_list[0].code = {0};
        return;
    }
    OrderedQueue q;
    std::vector<Node*> leaves;
    const size_t len = cell_list.size();
    for (size_t i = 0; i < len; ++i) {
        leaves.push_back(new Node(cell_list[i].cnt));
        q.Push(leaves.back());
    }
    while (q.Size() > 1) {
        Node* first = q.Top();
        q.Pop();
        Node* second = q.Top();
        q.Pop();
        std::swap(first, second);
        Node *root = new Node(first->sum + second->sum);
        root->l = first;
        root->r = second;
        first->parent = root;
        second->parent = root;
        q.Push(root);
    }
    for (size_t i = 0; i < len; ++i) {
        GetRootPath(leaves[i], cell_list[i].len);
    }
    auto compare_len = [](Cell a, Cell b) {
        return a.len < b.len;
    };
    std::sort(cell_list.begin(), cell_list.end(), compare_len);
    GetCanon(cell_list);
    head_ = q.Top();
    InlineClearTie();
}