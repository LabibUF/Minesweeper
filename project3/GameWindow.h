#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include <vector>

class GameWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    std::string playerName;

    sf::Sprite restartButton;
    sf::Sprite debugButton;
    sf::Sprite pauseButton;
    sf::Texture restartTexture;
    sf::Texture debugTexture;
    sf::Texture pauseTexture;
    sf::Texture winTexture;
    sf::Texture loseTexture;
    sf::Sprite leaderboardButton;
    sf::Texture leaderboardTexture;
    sf::Time pausedDuration;
    sf::Clock pauseClock;
    bool isPaused;

    sf::Texture digitsTexture;
    std::vector<sf::Sprite> counterDigits;
    std::vector<sf::Sprite> timerDigits;
    int remainingMines;

    Board board;
    bool gameOver;
    bool gameWon;
    bool paused;
    sf::Clock gameClock;
    sf::Text gameMessage;
    void openLeaderboard();

    void handleInput(sf::Event &event);
    void updateCounter();
    void updateTimer();
    void draw();

    void updateLeaderboard(const std::string &playerName, int timeInSeconds);
    void revealAllTiles();
    void resetTileStates();

public:
    GameWindow(int width, int height, int rows, int cols, int mines, const std::string &name);
    void run();
};

#endif GAME_WINDOW_H
