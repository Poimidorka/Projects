#pragma once

#include <algorithm>
#include <vector>
#include <queue>
#include "node.h"
#include "cell.h"

using CellList = std::vector<Cell>;

class TieArchiver {
public:
    void CodesByCnt(CellList& cell_list);
    TieArchiver();
    void Build(CellList& cells);
    ~TieArchiver() {
        InlineClearTie();
    }
    bool GetNext(bool c);
    void GetCanon(CellList& cell_list) const;
private:
    Node* head_;
    Node* current_;
    void InlineClearTie();
    void GetRootPath(Node *ptr, size_t& len) const;
};