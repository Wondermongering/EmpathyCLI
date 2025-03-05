#include "MoodTracker.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

using json = nlohmann::json;

// MoodEntry constructor
MoodEntry::MoodEntry(int score, const std::string& description) 
    : score(score), description(description), timestamp(std::chrono::system_clock::now()) {}

// MoodTracker constructor
MoodTracker::MoodTracker() {}

// Add a new mood entry to the history
void MoodTracker::addMoodEntry(int score, const std::string& description) {
    // Create and add the new entry
    moodHistory.emplace_back(score, description);
    
    // Extract individual words from the description and add to the set
    std::istringstream iss(description);
    std::string word;
    while (iss >> word) {
        // Convert to lowercase for consistency
        std::transform(word.begin(), word.end(), word.begin(), 
                      [](unsigned char c){ return std::tolower(c); });
        
        // Remove any punctuation
        word.erase(std::remove_if(word.begin(), word.end(), 
                                 [](unsigned char c){ return std::ispunct(c); }), 
                  word.end());
        
        // Add to set if it's not empty
        if (!word.empty()) {
            uniqueMoodWords.insert(word);
        }
    }
}

// Get the most recent mood entry
MoodEntry MoodTracker::getLatestMood() const {
    if (moodHistory.empty()) {
        return MoodEntry(5, "neutral"); // Default if no entries exist
    }
    return moodHistory.back();
}

// Get the entire mood history
const std::vector<MoodEntry>& MoodTracker::getMoodHistory() const {
    return moodHistory;
}

// Get all unique mood words that have been used
const std::set<std::string>& MoodTracker::getUniqueMoodWords() const {
    return uniqueMoodWords;
}

// Get average mood score over time
double MoodTracker::getAverageMoodScore() const {
    if (moodHistory.empty()) {
        return 5.0; // Default neutral score if no entries
    }
    
    double sum = 0.0;
    for (const auto& entry : moodHistory) {
        sum += entry.score;
    }
    
    return sum / moodHistory.size();
}

// Save mood history to a file
bool MoodTracker::saveMoodHistory(const std::string& filename) const {
    try {
        // Create JSON array to store entries
        json historyJson = json::array();
        
        for (const auto& entry : moodHistory) {
            // Convert timestamp to string
            auto timeT = std::chrono::system_clock::to_time_t(entry.timestamp);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S");
            
            // Create JSON object for this entry
            json entryJson = {
                {"score", entry.score},
                {"description", entry.description},
                {"timestamp", ss.str()}
            };
            
            historyJson.push_back(entryJson);
        }
        
        // Write JSON to file
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            return false;
        }
        
        outFile << std::setw(4) << historyJson << std::endl;
        return true;
    } catch (...) {
        return false;
    }
}

// Load mood history from a file
bool MoodTracker::loadMoodHistory(const std::string& filename) {
    try {
        // Open the file
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            return false;
        }
        
        // Parse JSON
        json historyJson;
        inFile >> historyJson;
        
        // Clear existing data
        moodHistory.clear();
        uniqueMoodWords.clear();
        
        // Load entries from JSON
        for (const auto& entryJson : historyJson) {
            int score = entryJson["score"];
            std::string description = entryJson["description"];
            
            // Create a new entry
            MoodEntry entry(score, description);
            
            // Parse timestamp (simplified - we're not restoring the exact timestamp)
            std::string timeStr = entryJson["timestamp"];
            std::tm tm = {};
            std::istringstream ss(timeStr);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            entry.timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
            
            // Add to history
            moodHistory.push_back(entry);
            
            // Process words for the unique set
            std::istringstream wordStream(description);
            std::string word;
            while (wordStream >> word) {
                // Convert to lowercase
                std::transform(word.begin(), word.end(), word.begin(), 
                              [](unsigned char c){ return std::tolower(c); });
                
                // Remove punctuation
                word.erase(std::remove_if(word.begin(), word.end(), 
                                         [](unsigned char c){ return std::ispunct(c); }), 
                          word.end());
                
                if (!word.empty()) {
                    uniqueMoodWords.insert(word);
                }
            }
        }
        
        return true;
    } catch (...) {
        return false;
    }
}
