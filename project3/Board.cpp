#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board(int rows, int cols, int mines)
    : rows(rows), cols(cols), mineCount(mines), flagsPlaced(0), minesPlaced(false) {
    tiles.resize(rows, std::vector<Tile>(cols));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            tiles[r][c].setPosition(c * 32, r * 32);
        }
    }
}

void Board::loadTextures() {
    hiddenTexture.loadFromFile("files/images/tile_hidden.png");
    flagTexture.loadFromFile("files/images/flag.png");
    mineTexture.loadFromFile("files/images/mine.png");

    for (int i = 1; i <= 8; ++i) {
        sf::Texture texture;
        texture.loadFromFile("files/images/number_" + std::to_string(i) + ".png");
        numberTextures.push_back(texture);
    }
}

void Board::placeMines(int startRow, int startCol) {
    std::srand(static_cast<unsigned>(std::time(0)));
    int placed = 0;

    while (placed < mineCount) {
        int r = std::rand() % rows;
        int c = std::rand() % cols;

        if (!tiles[r][c].hasMine() && !(r == startRow && c == startCol)) {
            tiles[r][c].setMine(true);
            ++placed;
        }
    }

    calculateAdjacency();
    minesPlaced = true;
}

void Board::calculateAdjacency() {
    const std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (tiles[r][c].hasMine()) continue;

            int count = 0;
            for (const auto &dir : directions) {
                int nr = r + dir.first, nc = c + dir.second;
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && tiles[nr][nc].hasMine()) {
                    ++count;
                }
            }
            tiles[r][c].setAdjacentMines(count);
        }
    }
}

void Board::draw(sf::RenderWindow &window) {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            tiles[r][c].loadTexture(hiddenTexture, flagTexture, mineTexture, numberTextures);
            tiles[r][c].draw(window);
        }
    }
}


bool Board::revealTile(int row, int col) {
    if (!minesPlaced) {
        placeMines(row, col);
    }

    if (row < 0 || row >= rows || col < 0 || col >= cols || tiles[row][col].isRevealedTile()) {
        return true;
    }

    tiles[row][col].reveal();
    if (tiles[row][col].hasMine()) {
        return false;
    }

    if (tiles[row][col].getAdjacentMines() == 0) {
        const std::vector<std::pair<int, int>> directions = {
            {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
        for (const auto &dir : directions) {
            revealTile(row + dir.first, col + dir.second);
        }
    }
    return true;
}

void Board::toggleFlag(int row, int col) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        Tile &tile = tiles[row][col];


        if (!tile.isRevealedTile()) {
            tile.toggleFlag();
            flagsPlaced += tile.isFlaggedTile() ? 1 : -1;
        }
    }
}


bool Board::checkVictory() const {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!tiles[r][c].hasMine() && !tiles[r][c].isRevealedTile()) {
                return false;
            }
        }
    }
    return true;
}

int Board::getRemainingMines() const {
    return mineCount - flagsPlaced;
}

void Board::toggleDebugMode() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (tiles[r][c].hasMine()) {
                tiles[r][c].reveal();
            }
        }
    }
}

Tile& Board::getTile(int row, int col) {
    return tiles[row][col];
}

void Board::resetTiles() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            tiles[r][c].reset();
        }
    }
}
