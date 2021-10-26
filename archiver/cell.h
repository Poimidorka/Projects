#pragma once

#include <cstdint>
#include <vector>

using BigCode = std::vector<bool>;

struct Cell {
    int32_t c = -1;
    size_t cnt = 0;
    size_t len = 0;
    BigCode code;
    Cell();
    Cell(int32_t c, size_t cnt);
    Cell(int32_t c);
};