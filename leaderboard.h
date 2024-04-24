//
// Created by lcadmin on 4/24/24.
//

#ifndef MINESWEEPER_LEADERBOARD_H
#define MINESWEEPER_LEADERBOARD_H

#include <chrono>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

typedef std::chrono::duration<std::time_t> Duration;
typedef std::chrono::minutes Minutes;
typedef std::chrono::seconds Seconds;

struct LeaderboardEntry {
    std::string name;
    Duration time;

    [[nodiscard]]
    Minutes::rep minutes() const {
        return std::chrono::duration_cast<Minutes>(time).count() % 99;
    }

    [[nodiscard]]
    Seconds::rep seconds() const {
        return std::chrono::duration_cast<Seconds>(time).count() % 60;
    }
};



std::vector<LeaderboardEntry> read_leaderboard(const char* path) {
    using namespace std;

    ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open leaderboard file!");
    }

    vector<LeaderboardEntry> entries;
    while (file.peek() != EOF) {
        string min_s, sec_s, name;
        getline(file, min_s, ':');
        getline(file, sec_s, ',');
        getline(file, name);

        const auto minutes = chrono::minutes(stoi(min_s));
        const auto seconds = chrono::seconds(stoi(sec_s));
        const auto duration = std::chrono::duration_cast<Duration>(minutes + seconds);
        entries.push_back(LeaderboardEntry{ name.substr(1), duration });
    }

    file.close();

    return entries;
}

bool sort_entries(LeaderboardEntry& lhs, LeaderboardEntry& rhs) {
    return lhs.time < rhs.time;
}

void update_leaderboard(const char* path, const LeaderboardEntry& newEntry) {
    using namespace std;

    auto leaderboard = read_leaderboard(path);
    leaderboard.emplace_back(newEntry);
    sort(leaderboard.begin(), leaderboard.end(), sort_entries);

    ofstream file(path, ios_base::out | ios_base::trunc);
    if (!file.is_open()) {
        throw runtime_error("Failed to open leaderboard file!");
    }

    for (auto i = 0; i < leaderboard.size() && i < 5; i++) {
        const auto& entry = leaderboard[i];
        file << setw(2) << setfill('0') << entry.minutes();
        file << ":";
        file << setw(2) << setfill('0') << entry.seconds();
        file << ", " << entry.name << std::endl;
    }

    file.close();
}


#endif //MINESWEEPER_LEADERBOARD_H
