#include "GameWindow.h"
#include <sstream>
#include <fstream>
#include <iomanip>

GameWindow::GameWindow(int width, int height, int rows, int cols, int mines, const std::string &name)
    : window(sf::VideoMode(width, height), "Minesweeper"),
      board(rows, cols, mines), playerName(name), gameOver(false), gameWon(false), paused(false),
      pausedDuration(sf::Time::Zero), isPaused(false) {

    // Load font and initialize other resources
    if (!font.loadFromFile("files/font.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    if (!restartTexture.loadFromFile("files/images/face_happy.png") ||
        !winTexture.loadFromFile("files/images/face_win.png") ||
        !loseTexture.loadFromFile("files/images/face_lose.png") ||
        !leaderboardTexture.loadFromFile("files/images/leaderboard.png")) {
        throw std::runtime_error("Failed to load one or more textures.");
    }

    debugTexture.loadFromFile("files/images/debug.png");
    pauseTexture.loadFromFile("files/images/pause.png");
    digitsTexture.loadFromFile("files/images/digits.png");

    // Happy Face Button
    restartButton.setTexture(restartTexture);
    restartButton.setPosition((cols * 32) / 2.0f - 32, 32 * (rows + 0.5));

    // Debug Button
    debugButton.setTexture(debugTexture);
    debugButton.setPosition((cols * 32) - 304, 32 * (rows + 0.5));

    // Pause Button
    pauseButton.setTexture(pauseTexture);
    pauseButton.setPosition((cols * 32) - 240, 32 * (rows + 0.5));

    // Leaderboard Button
    leaderboardButton.setTexture(leaderboardTexture);
    leaderboardButton.setPosition((cols * 32) - 176, 32 * (rows + 0.5)); // Add this line

    // Initialize board textures and tiles
    board.loadTextures();

    // Initialize counters
    remainingMines = board.getRemainingMines();
    counterDigits.resize(3);
    for (int i = 0; i < 3; ++i) {
        counterDigits[i].setTexture(digitsTexture);
        counterDigits[i].setPosition(33 + i * 21, 32 * (rows + 0.5) + 16);
    }
    updateCounter();

    // Initialize timer
    timerDigits.resize(4);
    for (int i = 0; i < 2; ++i) {
        // Timer Minutes
        timerDigits[i].setTexture(digitsTexture);
        timerDigits[i].setPosition((cols * 32) - 97 + i * 21, 32 * (rows + 0.5) + 16);

        // Timer Seconds
        timerDigits[i + 2].setTexture(digitsTexture);
        timerDigits[i + 2].setPosition((cols * 32) - 54 + i * 21, 32 * (rows + 0.5) + 16);
    }
}




void GameWindow::handleInput(sf::Event &event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Restart Button Click
        if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            gameOver = false;
            gameWon = false;
            paused = false;

            restartButton.setTexture(restartTexture); // Reset to happy face
            board = Board(board.getRows(), board.getCols(), board.getMineCount()); // Reset board
            board.loadTextures();
            gameClock.restart(); // Restart the timer
            remainingMines = board.getRemainingMines();
            updateCounter();
        }



        // Debug Button Click
        if (debugButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            board.toggleDebugMode();
        }

        // Pause Button Click
        if (pauseButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (!isPaused) {
                isPaused = true;
                pauseClock.restart(); // Start tracking pause time
            } else {
                isPaused = false;
                pausedDuration += pauseClock.getElapsedTime(); // Accumulate paused time
            }
        }

        if (leaderboardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            openLeaderboard();
        }

        // Game tile clicks
        if (!gameOver && !paused) {
            int row = mousePos.y / 32;
            int col = mousePos.x / 32;

            if (event.mouseButton.button == sf::Mouse::Left) {
                if (!board.revealTile(row, col)) {
                    gameOver = true;
                    restartButton.setTexture(loseTexture); // Set lose face
                } else if (board.checkVictory()) {
                    gameWon = true;
                    gameOver = true;
                    restartButton.setTexture(winTexture); // Set win face

                    // Automatically flag remaining mines
                    for (int r = 0; r < board.getRows(); ++r) {
                        for (int c = 0; c < board.getCols(); ++c) {
                            Tile& tile = board.getTile(r, c);
                            if (tile.hasMine() && !tile.isFlaggedTile()) {
                                tile.toggleFlag(); // Flag the tile
                            }
                        }
                    }

                    int elapsed = static_cast<int>((gameClock.getElapsedTime() - pausedDuration).asSeconds());
                    updateLeaderboard(playerName, elapsed); // Replace "PlayerName" with the actual player name
                }
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                board.toggleFlag(row, col);
                remainingMines = 0;
                updateCounter();
            }
        }
    }
}

void GameWindow::updateCounter() {
    int value = remainingMines;
    for (int i = 2; i >= 0; --i) {
        int digit = abs(value) % 10;
        counterDigits[i].setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        value /= 10;
    }

    // Display '-' for negative values
    if (remainingMines < 0) {
        counterDigits[0].setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
    }
}

void GameWindow::updateTimer() {
    if (paused) return;

    int elapsed = static_cast<int>((gameClock.getElapsedTime() - pausedDuration).asSeconds());

    // Extract minutes and seconds
    int minutes = elapsed / 60;
    int seconds = elapsed % 60;

    // Display minutes (2 digits)
    for (int i = 1; i >= 0; --i) {
        int digit = minutes % 10;
        timerDigits[i].setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        minutes /= 10;
    }

    // Display seconds (2 digits)
    for (int i = 3; i >= 2; --i) {
        int digit = seconds % 10;
        timerDigits[i].setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        seconds /= 10;
    }
}

void GameWindow::run() {
    gameClock.restart();

    // Render the initial state of the game
    draw();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleInput(event);
        }

        if (!paused && !gameOver) {
            updateTimer();
        }

        draw();
    }
}


void GameWindow::draw() {
    window.clear(sf::Color::White);

    // Draw the board
    board.draw(window);

    // Draw the restart button (face sprite)
    window.draw(restartButton);

    window.draw(leaderboardButton);

    // Draw other UI elements
    for (auto &digit : counterDigits) {
        window.draw(digit);
    }
    for (auto &digit : timerDigits) {
        window.draw(digit);
    }
    window.draw(debugButton);
    window.draw(pauseButton);

    if (gameOver) {
        gameMessage.setString(gameWon ? "You Win!" : "Game Over!");
        gameMessage.setPosition(window.getSize().x / 2 - gameMessage.getLocalBounds().width / 2,
                                window.getSize().y / 2 - gameMessage.getLocalBounds().height / 2);
        window.draw(gameMessage);
    }

    window.display();
}

void GameWindow::openLeaderboard() {
    // Pause the game timer
    sf::Time elapsedBeforePause = gameClock.getElapsedTime(); // Record the elapsed time
    paused = true; // Pause the game logic

    // Create a new window for the leaderboard
    sf::RenderWindow leaderboardWindow(sf::VideoMode(400, 300), "Leaderboard");

    // Load and set up the title text
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("LEADERBOARD");
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setPosition(400 / 2, 300 / 2 - 120);
    titleText.setOrigin(titleText.getLocalBounds().width / 2.0f, titleText.getLocalBounds().height / 2.0f);

    // Load and display the leaderboard content
    sf::Text leaderboardContent;
    leaderboardContent.setFont(font);
    leaderboardContent.setCharacterSize(18);
    leaderboardContent.setFillColor(sf::Color::White);

    std::ifstream file("files/leaderboard.txt");
    std::string line;
    std::string content;
    int rank = 1;

    while (std::getline(file, line) && rank <= 5) {
        size_t delimiter = line.find(" ");
        if (delimiter != std::string::npos) {
            std::string timeStr = line.substr(0, delimiter);
            std::string name = line.substr(delimiter + 1);

            // Add an asterisk if the name matches the playerName with an asterisk
            if (name == playerName) {
                name += "*";
            }

            // Add formatted leaderboard entry
            content += std::to_string(rank++) + ". " + timeStr + " " + name + "\n";
        }
    }
    file.close();

    leaderboardContent.setString(content);
    leaderboardContent.setPosition(400 / 2, 300 / 2 + 20);
    leaderboardContent.setOrigin(leaderboardContent.getLocalBounds().width / 2.0f, leaderboardContent.getLocalBounds().height / 2.0f);

    // Render the leaderboard window
    while (leaderboardWindow.isOpen()) {
        sf::Event event;
        while (leaderboardWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                leaderboardWindow.close();
            }
        }

        leaderboardWindow.clear(sf::Color::Blue);
        leaderboardWindow.draw(titleText);
        leaderboardWindow.draw(leaderboardContent);
        leaderboardWindow.display();
    }

    // Resume the game timer
    paused = false;
    gameClock.restart();
    pausedDuration += gameClock.getElapsedTime() - elapsedBeforePause;
}




void GameWindow::updateLeaderboard(const std::string &playerName, int timeInSeconds) {
    std::vector<std::pair<int, std::string>> leaderboard;

    // Read the existing leaderboard
    std::ifstream file("files/leaderboard.txt");
    std::string line;
    while (std::getline(file, line)) {
        size_t delimiter = line.find(" ");
        if (delimiter != std::string::npos) {
            std::string timeStr = line.substr(0, delimiter);
            std::string name = line.substr(delimiter + 1);

            // Parse MM:SS to total seconds
            size_t colonPos = timeStr.find(":");
            if (colonPos != std::string::npos) {
                int minutes = std::stoi(timeStr.substr(0, colonPos));
                int seconds = std::stoi(timeStr.substr(colonPos + 1));
                int totalSeconds = minutes * 60 + seconds;
                leaderboard.emplace_back(totalSeconds, name);
            }
        }
    }
    file.close();

    // Add the new score
    leaderboard.emplace_back(timeInSeconds, playerName);

    // Sort the leaderboard by time (ascending)
    std::sort(leaderboard.begin(), leaderboard.end());

    // Keep only the top 5
    if (leaderboard.size() > 5) {
        leaderboard.resize(5);
    }

    // Write the updated leaderboard back to the file
    std::ofstream outFile("files/leaderboard.txt");
    for (const auto &entry : leaderboard) {
        int minutes = entry.first / 60;
        int seconds = entry.first % 60;

        // Write MM:SS format and player name
        outFile << std::setfill('0') << std::setw(2) << minutes << ":"
                << std::setfill('0') << std::setw(2) << seconds << " "
                << entry.second << "\n";
    }
    outFile.close();
}

void GameWindow::revealAllTiles() {
    for (int r = 0; r < board.getRows(); ++r) {
        for (int c = 0; c < board.getCols(); ++c) {
            if (!board.getTile(r, c).isRevealedTile()) {
                board.getTile(r, c).setRevealed(true);
            }
        }
    }
}

void GameWindow::resetTileStates() {
    // Reset tiles to their prior state
    board.resetTiles();
}