#include "systems/ScoreManager.hpp"
#include "utils/AssetsUtils.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

const std::string ScoreManager::FILE_PATH = AssetPaths::ROOT / "scores.txt";

std::vector<ScoreEntry> ScoreManager::load() {
    std::vector<ScoreEntry> scores;
    std::ifstream file(FILE_PATH);

    if (!file.is_open())
        return scores;   // file does not exist yet — return empty

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        ScoreEntry entry;

        // Format per line: name score wins deaths
        if (ss >> entry.name >> entry.score >> entry.wins >> entry.deaths)
            scores.push_back(entry);
    }

    return scores;
}

void ScoreManager::save(const ScoreEntry& newEntry) {
    std::vector<ScoreEntry> scores = load();

    // Update existing entry if name matches, otherwise append
    bool found = false;
    for (ScoreEntry& e : scores) {
        if (e.name == newEntry.name) {
            e.score  += newEntry.score;
            e.wins   += newEntry.wins;
            e.deaths += newEntry.deaths;
            found = true;
            break;
        }
    }
    if (!found)
        scores.push_back(newEntry);

    write(scores);
}

void ScoreManager::write(const std::vector<ScoreEntry>& scores) {
    std::ofstream file(FILE_PATH);
    for (const ScoreEntry& e : scores)
        file << e.name  << " "
             << e.score << " "
             << e.wins  << " "
             << e.deaths << "\n";
}