//
// Created by lcadmin on 4/24/24.
//

#ifndef MINESWEEPER_GAME_WELCOME_H
#define MINESWEEPER_GAME_WELCOME_H

#include <SFML/Graphics.hpp>

#include "game.h"

#define BACKSPACE 8
#define ENTER 10
#define NAME_MAX_LEN 10

void Game::welcome_screen() {
    const auto width = config.cols * 32;
    const auto height = config.rows * 32 + 100;
    const sf::VideoMode mode(width, height);
    sf::RenderWindow window(mode, "Minesweeper", sf::Style::Close);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::TextEntered) {
                const auto key = static_cast<int>(event.text.unicode);

                if (key == BACKSPACE && !name.empty()) {
                    name.pop_back();
                } else if (key == ENTER && !name.empty()) {
                    return;
                } else if (isalpha(key) && name.length() < NAME_MAX_LEN) {
                    name.push_back(static_cast<char>(name.empty() ? toupper(key) : tolower(key)));
                }
            } else if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
        }

        window.clear(sf::Color::Blue);

        sf::Text welcome("WELCOME TO MINESWEEPER!", font);
        welcome.setFillColor(sf::Color::White);
        welcome.setCharacterSize(24);
        welcome.setStyle(sf::Text::Underlined);
        position_text(welcome, (float)width / 2, (float)height / 2 - 150);
        window.draw(welcome);

        sf::Text enterName("Enter your name:", font);
        enterName.setFillColor(sf::Color::White);
        enterName.setCharacterSize(20);
        position_text(enterName, (float)width / 2, (float)height / 2 - 75);
        window.draw(enterName);

        sf::Text userName(name + "|", font);
        userName.setFillColor(sf::Color::Yellow);
        userName.setCharacterSize(18);
        position_text(userName, (float)width / 2, (float)height / 2 - 45);
        window.draw(userName);

        window.display();
    }
}

#endif //MINESWEEPER_GAME_WELCOME_H
