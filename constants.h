//
// Created by lcadmin on 4/24/24.
//

#ifndef MINESWEEPER_CONSTANTS_H
#define MINESWEEPER_CONSTANTS_H

#include <SFML/Graphics.hpp>

#define CONFIG "files/config.cfg"
#define FONT "files/font.ttf"
#define LEADERBOARD "files/leaderboard.txt"

enum Button { happyFace, debug, pausePlay, leaderboard };

enum State {
    InProgress,
    Lose,
    Win,
};

void position_text(sf::Text& text, float x, float y) {
    const auto rect = text.getLocalBounds();
    text.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
    text.setPosition(x, y);
}

#endif //MINESWEEPER_CONSTANTS_H
