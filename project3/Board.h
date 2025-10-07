#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Board {
private:
    std::vector<std::vector<Tile>> tiles;
    int rows;
    int cols;
    int mineCount;
    int flagsPlaced;
    bool minesPlaced;
    sf::Texture hiddenTexture;
    sf::Texture flagTexture;
    sf::Texture mineTexture;
    std::vector<sf::Texture> numberTextures;

    void placeMines(int startRow, int startCol);
    void calculateAdjacency();

public:
    Board(int rows, int cols, int mines);
    void loadTextures();
    void draw(sf::RenderWindow &window);
    void toggleDebugMode();
    bool revealTile(int row, int col);
    void toggleFlag(int row, int col);
    bool checkVictory() const;
    int getRemainingMines() const;
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getMineCount() const { return mineCount; }
    Tile& getTile(int row, int col);
    void resetTiles();


};

#endif BOARD_H
