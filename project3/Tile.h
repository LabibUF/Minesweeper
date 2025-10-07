#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

class Tile {
private:
    sf::RectangleShape shape;
    sf::Sprite sprite;
    sf::Texture texture;

    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;

public:
    Tile();
    void setPosition(float x, float y);
    void setMine(bool mine);
    void toggleFlag();
    void reveal();
    void setAdjacentMines(int count);

    bool hasMine() const;
    bool isRevealedTile() const;
    bool isFlaggedTile() const;
    int getAdjacentMines() const;

    void loadTexture(const sf::Texture &hiddenTexture, const sf::Texture &flagTexture, const sf::Texture &mineTexture, const std::vector<sf::Texture> &numberTextures);
    void draw(sf::RenderWindow &window);
    void reset();
    void setRevealed(bool revealed);


};

#endif TILE_H
