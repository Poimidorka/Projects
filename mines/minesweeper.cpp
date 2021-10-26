#include <iostream>
#include "minesweeper.h"

void Minesweeper::DeletePole() {
    for (size_t i = 0; i < width_; ++i) {
        delete[] pole_[i];
    }
    delete[] pole_;
}

void Minesweeper::CreatePole() {
    pole_ = new Cell*[width_];
    for (size_t i = 0; i < width_; ++i) {
        pole_[i] = new Cell[height_];
        for (size_t j = 0; j < height_; ++j) {
            pole_[i][j].x = i;
            pole_[i][j].y = j;
        }
    }
}

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    width_ = width;
    height_ = height;
    all_time_ = 0;
    open_ = width * height;
    current_status_ = GameStatus::NOT_STARTED;
    std::vector<Cell> cells;
    mines_count_ = mines_count;
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            cells.push_back({.x = i, .y = j});
        }
    }
    shuffle(cells.begin(), cells.end(), generator_);
    CreatePole();
    for (size_t i = 0; i < mines_count; ++i) {
        pole_[cells[i].x][cells[i].y].is_mine = true;
    }
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    width_ = width;
    height_ = height;
    all_time_ = 0;
    open_ = width * height;
    mines_count_ = cells_with_mines.size();
    current_status_ = GameStatus::NOT_STARTED;
    CreatePole();
    for (auto& el : cells_with_mines) {
        pole_[el.x][el.y].is_mine = true;
    }
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    DeletePole();
    width_ = width;
    height_ = height;
    mines_count_ = mines_count;
    open_ = width * height;
    all_time_ = 0;
    current_status_ = GameStatus::NOT_STARTED;
    std::vector<Cell> cells;
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            cells.push_back({.x = i, .y = j});
        }
    }
    shuffle(cells.begin(), cells.end(), generator_);
    CreatePole();
    for (size_t i = 0; i < mines_count; ++i) {
        pole_[cells[i].x][cells[i].y].is_mine = true;
    }
}

bool Minesweeper::Correct(int32_t x, int32_t y) const {
    return x >= 0 && y >= 0 && static_cast<size_t>(x) < width_ && static_cast<size_t>(y) < height_;
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    DeletePole();
    mines_count_ = cells_with_mines.size();
    open_ = width * height;
    width_ = width;
    height_ = height;
    all_time_ = 0;
    current_status_ = GameStatus::NOT_STARTED;
    CreatePole();
    for (auto& el : cells_with_mines) {
        pole_[el.x][el.y].is_mine = true;
    }
}

std::vector<Minesweeper::Cell> Minesweeper::GetNear(Cell cell) const {
    std::vector<Cell> ans;
    int32_t x = static_cast<int32_t>(cell.x);
    int32_t y = static_cast<int32_t>(cell.y);
    for (int32_t i = -1; i <= 1; ++i) {
        for (int32_t j = -1; j <= 1; ++j) {
            if ((i != 0 || j != 0) && Correct(x + i, y + j)) {
                ans.push_back(pole_[x + i][y + j]);
            }
        }
    }
    return ans;
}

size_t Minesweeper::CountMines(Cell cell) const {
    auto list = GetNear(cell);
    size_t counter = 0;
    for (auto& cell : list) {
        counter += cell.is_mine;
    }
    return counter;
}

void Minesweeper::OpenCell(const Cell& cell) {
    if (current_status_ == GameStatus::DEFEAT || current_status_ == GameStatus::VICTORY) {
        return;
    }
    size_t x = cell.x;
    size_t y = cell.y;
    if (current_status_ == GameStatus::NOT_STARTED) {
        time(&start_time_);
        current_status_ = GameStatus::IN_PROGRESS;
    }
    if (pole_[x][y].is_mine) {
        current_status_ = GameStatus::DEFEAT;
        std::time_t now;
        time(&now);
        all_time_ += difftime(now, start_time_);
        return;
    }
    if (pole_[x][y].open || pole_[x][y].marked) {
        return;
    }
    std::queue<Cell> q;
    q.push(cell);
    pole_[x][y].open = true;
    while (!q.empty()) {
        Cell current = q.front();
        q.pop();
        --open_;
        if (CountMines(current) == 0) {
            for (Cell neib : GetNear(current)) {
                neib = pole_[neib.x][neib.y];
                if (!neib.open && !neib.is_mine && !neib.marked) {
                    q.push(neib);
                    pole_[neib.x][neib.y].open = true;
                }
            }
        }
    }
    if (open_ == mines_count_) {
        current_status_ = GameStatus::VICTORY;
        std::time_t now;
        time(&now);
        all_time_ += difftime(now, start_time_);
        return;
    }
}

void Minesweeper::MarkCell(const Cell& cell) {
    if (current_status_ == GameStatus::DEFEAT || current_status_ == GameStatus::VICTORY) {
        return;
    }
    size_t x = cell.x;
    size_t y = cell.y;
    if (current_status_ == GameStatus::NOT_STARTED) {
        time(&start_time_);
        current_status_ = GameStatus::IN_PROGRESS;
    }
    pole_[x][y].marked = !(pole_[x][y].marked);
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    RenderedField ans(height_, std::string(width_, '-'));
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            const Cell& cell = pole_[j][i];
            char& c = ans[i][j];
            if (cell.open) {
                c = static_cast<char>('0' + CountMines(cell));
            } else if (current_status_ == GameStatus::DEFEAT && cell.is_mine) {
                c = '*';
            } else if (cell.marked) {
                c = '?';
            }
            if (c == '0') {
                c = '.';
            }
        }
    }
    return ans;
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return current_status_;
}

time_t Minesweeper::GetGameTime() const {
    if (current_status_ == GameStatus::NOT_STARTED) {
        return 0;
    }
    time_t current;
    time(&current);
    size_t ans = static_cast<size_t>(all_time_ + difftime(current, start_time_));
    return static_cast<time_t>(ans);
}

Minesweeper::~Minesweeper() {
    DeletePole();
}