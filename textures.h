//
// Created by lcadmin on 4/24/24.
//

#ifndef MINESWEEPER_TEXTURES_H
#define MINESWEEPER_TEXTURES_H

#include <iostream>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "constants.h"

struct Textures {
    std::map<std::string, sf::Texture> textures;

    Textures() {
        const char* textureNames[19] = { "mine", "tile_hidden", "tile_revealed", "number_1", "number_2", "number_3", "number_4", "number_5", "number_6", "number_7", "number_8", "face_happy", "face_win", "face_lose", "debug", "pause", "play", "leaderboard", "flag" };
        for (const auto& textureName: textureNames) {
            sf::Texture texture;
            if (!texture.loadFromFile("files/images/" + std::string(textureName) + ".png")) {
                std::cerr << "Failed to load texture: " << textureName << ".png" << std::endl;
                exit(1);
            }

            textures[textureName] = texture;
        }

        for (auto i = 0; i < 11; i++) {
            sf::Texture digit;
            sf::IntRect rect(21 * i, 0, 21, 32);
            if (!digit.loadFromFile("files/images/digits.png", rect)) {
                std::cerr << "Failed to load texture for digit " << i << std::endl;
                exit(1);
            }

            textures["digit_" + std::to_string(i)] = digit;
        }
    }

    sf::Texture& mine() { return textures["mine"]; }

    sf::Texture& flag() { return textures["flag"]; }

    sf::Texture& tile_hidden() { return textures["tile_hidden"]; }

    sf::Texture& tile_revealed() { return textures["tile_revealed"]; }

    sf::Texture& number(unsigned int num) {
        return textures["number_" + std::to_string(num)];
    }

    sf::Texture& face_for_state(State state) {
        if (state == State::Lose) return face_lose();
        if (state == State::Win) return face_win();
        return face_happy();
    }

    sf::Texture& face_happy() { return textures["face_happy"]; }

    sf::Texture& face_win() { return textures["face_win"]; }

    sf::Texture& face_lose() { return textures["face_lose"]; }

    sf::Texture& digit(unsigned int num) {
        return textures["digit_" + std::to_string(num)];
    }

    sf::Texture& digit_dash() { return textures["digit_10"]; }

    sf::Texture& debug() { return textures["debug"]; }

    sf::Texture& pause() { return textures["pause"]; }

    sf::Texture& play() { return textures["play"]; }

    sf::Texture& leaderboard() { return textures["leaderboard"]; }
};


#endif //MINESWEEPER_TEXTURES_H
