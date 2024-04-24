//
// Created by lcadmin on 4/24/24.
//

#ifndef MINESWEEPER_GAME_GAME_H
#define MINESWEEPER_GAME_GAME_H

#include <chrono>
#include <SFML/Graphics.hpp>

#include "game.h"
#include "leaderboard.h"

void Game::game_screen() {
    start_time = std::chrono::high_resolution_clock::now();

    unsigned int width = config.cols * 32;
    unsigned int height = config.rows * 32 + 100;
    const sf::VideoMode mode(width, height);
    sf::RenderWindow window(mode, "Minesweeper", sf::Style::Close);

    bool debugEnabled = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                const auto button = event.mouseButton.button;
                const auto x = event.mouseButton.x;
                const auto y = event.mouseButton.y;

                const auto clicked = [this, x, y](Button button) {
                    const auto position = this->buttons<int>()[button];
                    return position.contains(x, y);
                };

                if (y < config.rows * 32) {
                    const auto col = x / 32;
                    const auto row = y / 32;
                    const auto index = row * config.cols + col;
                    auto& tile = tiles[index];

                    if (button == sf::Mouse::Button::Right) {
                        tile.flag();
                    } else {
                        if (tile.flagged) continue;
                        if (tile.has_mine) {
                            const auto now = std::chrono::high_resolution_clock::now();
                            elapsed += std::chrono::duration_cast<std::chrono::duration<std::time_t>>(now - start_time);
                            state = State::Lose;
                            tile.reveal();
                            continue;
                        }

                        tile.reveal();

                        for (const auto empty: find_empty_adjacent_tiles(index)) {
                            tiles[empty].reveal();
                        }
                    }
                } else {
                    if (clicked(Button::happyFace)) {
                        reset();
                    } else if (clicked(Button::debug) && state == State::InProgress && !paused) {
                        debugEnabled = !debugEnabled;
                    } else if (clicked(Button::pausePlay) && state == State::InProgress) {
                        if (paused) {
                            // Resume
                            start_time = std::chrono::high_resolution_clock::now();
                            paused = false;
                        } else {
                            // Pause
                            const auto now = std::chrono::high_resolution_clock::now();
                            elapsed += std::chrono::duration_cast<std::chrono::duration<std::time_t>>(now - start_time);
                            paused = true;
                        }
                    } else if (clicked(Button::leaderboard)) {
                        paused = true;
                        showing_leaderboard = true;
                        const auto now = std::chrono::high_resolution_clock::now();
                        elapsed += std::chrono::duration_cast<std::chrono::duration<std::time_t>>(now - start_time);
                    }
                }
                std::cout << x << ", " << y << std::endl;
            }
        }

        window.clear(sf::Color::White);

        auto btns = buttons<float>();

        sf::Texture faceTexture = textures.face_for_state(state);

        sf::Sprite faceSprite(faceTexture);
        faceSprite.setPosition(btns[Button::happyFace].getPosition());
        window.draw(faceSprite);

        sf::Sprite debugButton(textures.debug());
        debugButton.setPosition(btns[Button::debug].getPosition());
        window.draw(debugButton);

        sf::Sprite pausePlay = paused ? sf::Sprite(textures.play()) : sf::Sprite(textures.pause());
        pausePlay.setPosition(btns[Button::pausePlay].getPosition());
        window.draw(pausePlay);

        sf::Sprite leaderboard(textures.leaderboard());
        leaderboard.setPosition(btns[Button::leaderboard].getPosition());
        window.draw(leaderboard);

        const auto now = std::chrono::high_resolution_clock::now();
        auto el = elapsed.count() + std::chrono::duration_cast<std::chrono::duration<std::time_t>>(now - start_time).count();
        if (state != State::InProgress || paused) el = elapsed.count();

        auto minutes = (unsigned int)el / 60;
        auto seconds = (unsigned int)el % 60;

        sf::Sprite minutes_high(textures.digit(minutes / 10));
        minutes_high.setPosition(32.0f * (float)config.cols - 97, 32.0f * (float)config.rows + 34);
        window.draw(minutes_high);

        sf::Sprite minutes_low(textures.digit(minutes % 10));
        minutes_low.setPosition(32.0f * (float)config.cols - 76, 32.0f * (float)config.rows + 34);
        window.draw(minutes_low);

        sf::Sprite secH(textures.digit(seconds / 10));
        secH.setPosition(32.0f * (float)config.cols - 54, 32.0f * (float)config.rows + 34);
        window.draw(secH);

        sf::Sprite secL(textures.digit(seconds % 10));
        secL.setPosition(32.0f * (float)config.cols - 33, 32.0f * (float)config.rows + 34);
        window.draw(secL);

        int counter = 0;
        unsigned int remainingTiles = 0;

        for (auto i = 0; i < size(); i++) {
            const auto tile = tiles[i];
            const auto col = i % config.cols;
            const auto row = i / config.cols;
            const float x = 32.0f * (float)col;
            const float y = 32.0f * (float)row;

            if (tile.has_mine) counter++;
            if (tile.flagged) counter--;
            if (!tile.has_mine && !tile.revealed) remainingTiles++;

            sf::Sprite sprite(textures.tile_hidden());

            if (tile.revealed || showing_leaderboard || paused) {
                sprite = sf::Sprite(textures.tile_revealed());
            }

            sprite.setPosition(x, y);
            window.draw(sprite);

            if (paused || showing_leaderboard) continue;

            if (tile.revealed && tile.nearby_mines) {
                sf::Sprite number(textures.number(tile.nearby_mines));
                number.setPosition(x, y);
                window.draw(number);
            }

            if (tile.flagged) {
                sf::Sprite flag(textures.flag());
                flag.setPosition(x, y);
                window.draw(flag);
            }

            if ((debugEnabled || state == State::Lose) && tile.has_mine) {
                sf::Sprite mine(textures.mine());
                mine.setPosition(x, y);
                window.draw(mine);
            }
        }

        if (remainingTiles == 0) {
            if (state == State::InProgress) {
                elapsed += std::chrono::duration_cast<std::chrono::duration<std::time_t>>(now - start_time);
            }

            if (state != State::Win) {
                state = State::Win;

                update_leaderboard(LEADERBOARD, { name, elapsed });
            }
        }

        const auto highTexture = counter < 0
                                 ? textures.digit_dash()
                                 : textures.digit(counter / 100);
        sf::Sprite counter3(highTexture);
        counter3.setPosition(33, 32.0f * (float)config.rows + 32);
        window.draw(counter3);

        sf::Sprite counter2(textures.digit(abs(counter) / 10 % 10));
        counter2.setPosition(54, 32.0f * (float)config.rows + 32);
        window.draw(counter2);

        sf::Sprite counter1(textures.digit(abs(counter) % 10));
        counter1.setPosition(75, 32.0f * (float)config.rows + 32);
        window.draw(counter1);

        if (showing_leaderboard) {
            window.display();

            leaderboard_screen(window);
            start_time = std::chrono::high_resolution_clock::now();
            paused = false;
            showing_leaderboard = false;
        }

        window.display();
    }
}

#endif //MINESWEEPER_GAME_GAME_H
