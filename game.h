//
// Created by lcadmin on 4/24/24.
//

#ifndef MINESWEEPER_GAME_H
#define MINESWEEPER_GAME_H

#include <chrono>
#include <random>
#include <set>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "config.h"
#include "textures.h"
#include "tile.h"

std::vector<unsigned int> random_indices(unsigned int n, unsigned int count) {
    if (count > n) return {};

    std::vector<unsigned int> indices;
    indices.reserve(n);
    for (auto i = 0; i < n; i++) indices.push_back(i);

    std::shuffle(indices.begin(), indices.end(), std::random_device());

    return {indices.begin(), indices.begin() + count};
}

class Game {
    static sf::Font load_font(const char* path) {
        sf::Font font;
        font.loadFromFile(path);
        return font;
    }

    // SFML resources
    sf::Font font;
    Textures textures;

    // Player and board setup
    Config config;
    std::string name;

    // Game state
    bool paused = false;
    bool showing_leaderboard = false;
    State state = State::InProgress;
    std::vector<Tile> tiles;

    // Clock
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time{};
    std::chrono::duration<std::time_t> elapsed{};

    unsigned int size() const {
        return config.cols * config.rows;
    }

    std::vector<unsigned int> adjacent_tiles(unsigned int index) const {
        const auto cc = static_cast<int>(config.cols);

        const auto row = index / config.cols;
        const auto col = index % config.cols;

        std::vector<unsigned int> adjacent;
        if (row != 0) adjacent.push_back(index - cc);
        if (row != config.rows - 1) adjacent.push_back(index + cc);
        if (col != 0) adjacent.push_back(index - 1);
        if (col != config.cols - 1) adjacent.push_back(index + 1);
        return adjacent;
    }

    std::set<unsigned int> find_empty_adjacent_tiles(unsigned int index) const {
        std::set<unsigned int> empties;
        if (tiles[index].nearby_mines) return empties;
        empties.insert(index);
        return find_empty_adjacent_tiles(index, empties);
    }

    std::set<unsigned int> find_empty_adjacent_tiles(unsigned int index, std::set<unsigned int>& empties) const {
        const auto adjacent = adjacent_tiles(index);

        for (const auto adj: adjacent) {
            if (!tiles[adj].flagged && !tiles[adj].has_mine && !tiles[adj].revealed) {
                if (empties.contains(adj)) continue;
                empties.insert(adj);
                if (!tiles[adj].nearby_mines) find_empty_adjacent_tiles(adj, empties);
            }
        }

        return empties;
    }

    unsigned int count_nearby_mines(unsigned int index) const {
        const auto i = static_cast<int>(index);
        const auto cc = static_cast<int>(config.cols);

        int neighbors[8] = { i-cc-1, i-cc, i-cc+1,
                             i - 1,             i + 1,
                             i+cc-1, i+cc, i+cc+1 };

        const auto row = index / config.cols;
        const auto col = index % config.cols;

        if (row == 0) neighbors[0] = neighbors[1] = neighbors[2] = -1;
        if (row == config.rows - 1) neighbors[5] = neighbors[6] = neighbors[7] = -1;
        if (col == 0) neighbors[0] = neighbors[3] = neighbors[5] = -1;
        if (col == config.cols - 1) neighbors[2] = neighbors[4] = neighbors[7] = -1;

        unsigned int mine_count = 0;
        for (const auto neighbor: neighbors) {
            if (neighbor < 0) continue;
            if (tiles[neighbor].has_mine) mine_count++;
        }

        return mine_count;
    }

    Game(): config(Config::from_file(CONFIG)), font(Game::load_font(FONT)) {
        setup_tiles();
    }

    void setup_tiles() {
        tiles.clear();

        for (auto i = 0; i < size(); i++) {
            tiles.emplace_back();
        }

        for (const auto& index: random_indices(size(), config.mines)) {
            tiles[index].has_mine = true;
        }

        for (auto i = 0; i < size(); i++) {
            tiles[i].nearby_mines = count_nearby_mines(i);
        }
    }

    void reset() {
        state = State::InProgress;
        setup_tiles();

        elapsed = std::chrono::duration<std::time_t>();
        start_time = std::chrono::high_resolution_clock::now();
    }

    template<typename T>
    std::map<Button, sf::Rect<T>> buttons() const {
        std::map<Button, sf::Rect<T>> rects;
        rects[Button::debug] = sf::Rect<T>(
                static_cast<T>(32.0f * (float)config.cols - 304.0f),
                static_cast<T>(32.0f * (float)config.rows + 16.0f),
                64, 64);
        rects[Button::happyFace] = sf::Rect<T>(
                static_cast<T>(32.0f * (float)config.cols / 2.0f - 32.0f),
                static_cast<T>(32.0f * (float)config.rows + 16.0f),
                64, 64);
        rects[Button::leaderboard] = sf::Rect<T>(
                static_cast<T>(32.0f * (float)config.cols - 176.0f),
                static_cast<T>(32.0f * (float)config.rows + 16.0f),
                64, 64);
        rects[Button::pausePlay] = sf::Rect<T>(
                static_cast<T>(32.0f * (float)config.cols - 240.0f),
                static_cast<T>(32.0f * (float)config.rows + 16.0f),
                64, 64);
        return rects;
    }

    void welcome_screen();
    void leaderboard_screen(sf::RenderWindow& parent);
    void game_screen();

public:
    static void begin() {
        Game game;
        game.welcome_screen();
        game.game_screen();
    }
};

#include "game+game.h"
#include "game+leaderboard.h"
#include "game+welcome.h"

#endif //MINESWEEPER_GAME_H
