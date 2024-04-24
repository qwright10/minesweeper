//
// Created by lcadmin on 4/24/24.
//

#ifndef MINESWEEPER_TILE_H
#define MINESWEEPER_TILE_H

struct Tile {
    bool flagged;
    bool has_mine;
    bool revealed;
    unsigned int nearby_mines = 0;

    Tile(): flagged(false), has_mine(false), revealed(false) {}

    Tile& flag() {
        if (!revealed) flagged = !flagged;
        return *this;
    }

    Tile& flag(bool isFlagged) {
        flagged = isFlagged;
        return *this;
    }

    Tile& reveal() {
        flagged = false;
        revealed = true;
        return *this;
    }
};

#endif //MINESWEEPER_TILE_H
