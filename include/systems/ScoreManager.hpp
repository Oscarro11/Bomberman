#pragma once
#include <vector>
#include <string>

struct ScoreEntry {
    std::string name;
    int         score;
    int         wins;
    int         deaths;
};

class ScoreManager {
    public:
        static const std::string FILE_PATH;

        // Load all entries from file — returns empty vector if file missing
        static std::vector<ScoreEntry> load();

        // Append or update an entry and save back to file
        static void save(const ScoreEntry& entry);

    private:
        static void write(const std::vector<ScoreEntry>& scores);
};