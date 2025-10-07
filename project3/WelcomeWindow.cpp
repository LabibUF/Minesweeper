#include "WelcomeWindow.h"
#include <cctype>

WelcomeWindow::WelcomeWindow(int width, int height)
    : window(sf::VideoMode(width, height), "Welcome to Minesweeper!"),
      cursorVisible(true) {
    // Load the font
    if (!font.loadFromFile("files/font.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    // Set up title text
    titleText.setFont(font);
    titleText.setString("WELCOME TO MINESWEEPER!");
    titleText.setCharacterSize(24);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(width / 2.0f, height / 2.0f - 150);
    titleText.setOrigin(titleText.getLocalBounds().width / 2.0f, titleText.getLocalBounds().height / 2.0f);

    // Set up prompt text
    promptText.setFont(font);
    promptText.setString("Enter your name:");
    promptText.setCharacterSize(20);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(width / 2.0f, height / 2.0f - 75);
    promptText.setOrigin(promptText.getLocalBounds().width / 2.0f, promptText.getLocalBounds().height / 2.0f);

    // Set up player name text
    nameText.setFont(font);
    nameText.setCharacterSize(18);
    nameText.setFillColor(sf::Color::Yellow);
    nameText.setPosition(width / 2.0f, height / 2.0f - 45);

    // Set up cursor text
    cursorText.setFont(font);
    cursorText.setString("|");
    cursorText.setCharacterSize(18);
    cursorText.setFillColor(sf::Color::Yellow);
    cursorText.setPosition(nameText.getPosition().x, nameText.getPosition().y);
}

void WelcomeWindow::handleInput(sf::Event &event) {
    if (event.type == sf::Event::TextEntered) {
        if (isalpha(event.text.unicode) && playerName.size() < 10) {
            char c = static_cast<char>(event.text.unicode);
            if (playerName.empty()) {
                c = toupper(c);
            } else {
                c = tolower(c);
            }
            playerName += c;
        } else if (event.text.unicode == '\b' && !playerName.empty()) {
            playerName.pop_back();
        }

        // Center the name text
        sf::FloatRect textBounds = nameText.getLocalBounds();
        nameText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
        nameText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45);
    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter && !playerName.empty()) {
            window.close();
        } else if (event.key.code == sf::Keyboard::Escape) {
            window.close();
            playerName.clear();
        }
    }
}

void WelcomeWindow::updateCursor() {
    if (cursorTimer.getElapsedTime().asMilliseconds() > 500) {
        cursorVisible = !cursorVisible;
        cursorTimer.restart();
    }
    cursorText.setPosition(nameText.getPosition().x + nameText.getLocalBounds().width + 5, nameText.getPosition().y);
}

void WelcomeWindow::draw() {
    window.clear(sf::Color::Blue);
    window.draw(titleText);
    window.draw(promptText);
    nameText.setString(playerName);
    window.draw(nameText);
    if (cursorVisible) {
        window.draw(cursorText);
    }
    window.display();
}

std::string WelcomeWindow::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                playerName.clear();
            }
            handleInput(event);
        }
        updateCursor();
        draw();
    }
    return playerName;
}
