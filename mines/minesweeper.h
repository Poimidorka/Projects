#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <queue>

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
        bool is_mine = false;
        bool marked = false;
        bool open = false;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

    ~Minesweeper();

private:
    Cell** pole_;

    size_t width_;
    size_t height_;

    std::time_t start_time_;
    std::mt19937 generator_;
    GameStatus current_status_;
    double all_time_;

    size_t mines_count_;
    size_t open_;

    bool Correct(int32_t x, int32_t y) const;
    size_t CountMines(Cell cell) const;
    std::vector<Cell> GetNear(Cell cell) const;
    void CreatePole();
    void DeletePole();
};
