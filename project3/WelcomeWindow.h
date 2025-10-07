#ifndef WELCOME_WINDOW_H
#define WELCOME_WINDOW_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

class WelcomeWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::Text promptText;
    sf::Text nameText;
    sf::Text cursorText;

    string playerName;
    bool cursorVisible;
    sf::Clock cursorTimer;

    void handleInput(sf::Event &event);
    void updateCursor();
    void draw();

public:
    WelcomeWindow(int width, int height);
    string run();
};

#endif WELCOME_WINDOW_H

