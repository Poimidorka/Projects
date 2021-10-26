#include "cell.h"

Cell::Cell() : c(-1), cnt(0), len(0) {
}

Cell::Cell(int32_t c) : c(c) {
}

Cell::Cell(int32_t c, size_t cnt) : c(c), cnt(cnt) {
}