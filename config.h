//
// Created by lcadmin on 4/24/24.
//

#ifndef MINESWEEPER_CONFIG_H
#define MINESWEEPER_CONFIG_H

#include <fstream>

struct Config {
    unsigned int cols;
    unsigned int rows;
    unsigned int mines;

    static Config from_file(const char* path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Config file not found!");
        }

        std::string cols_s, rows_s, mines_s;
        getline(file, cols_s);
        getline(file, rows_s);
        getline(file, mines_s);

        int colCount = std::stoi(cols_s);
        int rowCount = std::stoi(rows_s);
        int mineCount = std::stoi(mines_s);

        return Config {
                static_cast<unsigned int>(colCount),
                static_cast<unsigned int>(rowCount),
                static_cast<unsigned int>(mineCount),
        };
    }
};


#endif //MINESWEEPER_CONFIG_H
