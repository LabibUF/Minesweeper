#include "Tile.h"

Tile::Tile() : isMine(false), isRevealed(false), isFlagged(false), adjacentMines(0) {
    shape.setSize(sf::Vector2f(32, 32));
    shape.setFillColor(sf::Color::White);
}

void Tile::setPosition(float x, float y) {
    shape.setPosition(x, y);
    sprite.setPosition(x, y);
}

void Tile::setMine(bool mine) {
    isMine = mine;
}

void Tile::toggleFlag() {
    if (!isRevealed) {
        isFlagged = !isFlagged;
    }
}

void Tile::reveal() {
    if (!isFlagged) {
        isRevealed = true;
    }
}

void Tile::setAdjacentMines(int count) {
    adjacentMines = count;
}

bool Tile::hasMine() const {
    return isMine;
}

bool Tile::isRevealedTile() const {
    return isRevealed;
}

bool Tile::isFlaggedTile() const {
    return isFlagged;
}

int Tile::getAdjacentMines() const {
    return adjacentMines;
}

void Tile::loadTexture(const sf::Texture &hiddenTexture, const sf::Texture &flagTexture, const sf::Texture &mineTexture, const std::vector<sf::Texture> &numberTextures) {
    if (!isRevealed) {
        if (isFlagged) {
            sprite.setTexture(flagTexture);
        } else {
            sprite.setTexture(hiddenTexture);
        }
    } else {
        if (isMine) {
            sprite.setTexture(mineTexture);
        } else if (adjacentMines > 0) {
            sprite.setTexture(numberTextures[adjacentMines - 1]);
        }
    }
}

void Tile::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}

void Tile::reset() {
    isMine = false;
    isRevealed = false;
    isFlagged = false;
    adjacentMines = 0;
}

void Tile::setRevealed(bool revealed) {
    isRevealed = revealed;
}
