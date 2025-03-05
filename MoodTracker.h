#ifndef MOOD_TRACKER_H
#define MOOD_TRACKER_H

#include <vector>
#include <string>
#include <set>
#include <chrono>

// Structure to store mood entries
struct MoodEntry {
    int score;                                              // Numerical score from 1-10
    std::string description;                                // Text description of mood
    std::chrono::system_clock::time_point timestamp;        // When the entry was recorded
    
    // Constructor for easy creation
    MoodEntry(int score, const std::string& description);
};

class MoodTracker {
private:
    std::vector<MoodEntry> moodHistory;                     // Vector to store mood history
    std::set<std::string> uniqueMoodWords;                  // Set to track unique mood descriptors

public:
    MoodTracker();
    
    // Add a new mood entry to the history
    void addMoodEntry(int score, const std::string& description);
    
    // Get the most recent mood entry
    MoodEntry getLatestMood() const;
    
    // Get the entire mood history
    const std::vector<MoodEntry>& getMoodHistory() const;
    
    // Get all unique mood words that have been used
    const std::set<std::string>& getUniqueMoodWords() const;
    
    // Get average mood score over time
    double getAverageMoodScore() const;
    
    // Save mood history to a file
    bool saveMoodHistory(const std::string& filename) const;
    
    // Load mood history from a file
    bool loadMoodHistory(const std::string& filename);
};

#endif // MOOD_TRACKER_H
