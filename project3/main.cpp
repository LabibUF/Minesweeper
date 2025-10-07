#include "WelcomeWindow.h"
#include "GameWindow.h"
#include <iostream>

int main() {
    try {
        // Launch the Welcome Window
        WelcomeWindow welcome(800, 600);
        std::string playerName = welcome.run();

        if (!playerName.empty()) {
            std::cout << "Player Name: " << playerName << std::endl;

            // Game Configuration
            int rows = 16;
            int cols = 25;
            int mines = 50;

            // Calculate Game Window dimensions
            int width = cols * 32;
            int height = rows * 32 + 100;

            // Launch the Game Window
            GameWindow game(width, height, rows, cols, mines, playerName);
            game.run();
        } else {
            std::cout << "Game exited from Welcome Window." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}