//
// Created by lcadmin on 4/24/24.
//

#ifndef MINESWEEPER_GAME_LEADERBOARD_H
#define MINESWEEPER_GAME_LEADERBOARD_H

#include <sstream>
#include <SFML/Graphics.hpp>

#include "game.h"

void Game::leaderboard_screen(sf::RenderWindow& parent) {
    unsigned int width = config.cols * 16;
    unsigned int height = config.rows * 16 + 50;
    const sf::VideoMode mode(width, height);
    sf::RenderWindow window(mode, "Minesweeper", sf::Style::Close);

    const auto lb = read_leaderboard(LEADERBOARD);

    while (window.isOpen()) {
        sf::Event event;
        while (parent.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                parent.close();
                exit(0);
            }
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }

        window.clear(sf::Color::Blue);

        sf::Text leaderboard_text("LEADERBOARD", font, 20);
        leaderboard_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
        position_text(leaderboard_text, (float) width / 2.0f, (float) height / 2.0f - 120);
        window.draw(leaderboard_text);

        std::stringstream leaderboard_output;
        for (auto i = 0; i < lb.size(); i++) {
            using namespace std;

            const auto &entry = lb[i];
            leaderboard_output << i + 1 << ".\t";
            leaderboard_output << setw(2) << setfill('0') << entry.minutes();
            leaderboard_output << ":";
            leaderboard_output << setw(2) << setfill('0') << entry.seconds();
            leaderboard_output << "\t" << entry.name;
            if (name == entry.name) leaderboard_output << "*";
            leaderboard_output << "\n\n";
        }

        sf::Text lb_contents(leaderboard_output.str(), font, 18);
        lb_contents.setStyle(sf::Text::Bold);
        position_text(lb_contents, (float) width / 2, (float) height / 2 + 20);
        window.draw(lb_contents);

        window.display();
    }
}

#endif //MINESWEEPER_GAME_LEADERBOARD_H
