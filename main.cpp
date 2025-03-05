#include "MoodTracker.h"
#include "ResourceMap.h"
#include <iostream>
#include <string>
#include <limits>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <random>

namespace fs = std::filesystem;

// Function declarations
void displayWelcomeMessage();
void displayMenu();
void addNewMoodEntry(MoodTracker& tracker, const ResourceMap& resources);
void viewMoodHistory(const MoodTracker& tracker);
void viewMoodStats(const MoodTracker& tracker);
void displayResources(const ResourceMap& resources);
void clearScreen();
bool confirmAction(const std::string& message);
void displayResourcesBasedOnMood(const MoodEntry& entry, const ResourceMap& resources);
std::string formatTimestamp(const std::chrono::system_clock::time_point& timestamp);
std::string getRandomEncouragement();

int main() {
    // Initialize the mood tracker and resource map
    MoodTracker tracker;
    ResourceMap resources;
    
    // Create data directory if it doesn't exist
    std::string dataPath = "data";
    if (!fs::exists(dataPath)) {
        fs::create_directory(dataPath);
    }
    
    // Try to load existing mood history
    std::string historyFile = dataPath + "/mood_history.json";
    if (fs::exists(historyFile)) {
        tracker.loadMoodHistory(historyFile);
    }
    
    // Try to load custom resources
    std::string resourceFile = "resources/empathylinks.json";
    if (fs::exists(resourceFile)) {
        resources.loadResourcesFromFile(resourceFile);
    }
    
    // Display welcome message
    displayWelcomeMessage();
    
    // Main program loop
    bool running = true;
    while (running) {
        displayMenu();
        
        int choice;
        std::cout << "Enter your choice (1-5): ";
        
        if (!(std::cin >> choice)) {
            // Clear the error state
            std::cin.clear();
            // Skip bad input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
        
        // Clear any remaining characters in input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                // Add new mood entry
                addNewMoodEntry(tracker, resources);
                
                // Save after each entry
                tracker.saveMoodHistory(historyFile);
                break;
            case 2:
                // View mood history
                viewMoodHistory(tracker);
                break;
            case 3:
                // View mood statistics
                viewMoodStats(tracker);
                break;
            case 4:
                // View all resources
                displayResources(resources);
                break;
            case 5:
                // Exit
                if (confirmAction("Are you sure you want to exit? (y/n): ")) {
                    running = false;
                    std::cout << "Thank you for using EmpathyCLI. Take care!" << std::endl;
                }
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
    
    return 0;
}

// Function to display welcome message
void displayWelcomeMessage() {
    clearScreen();
    std::cout << "==============================================" << std::endl;
    std::cout << "             Welcome to EmpathyCLI            " << std::endl;
    std::cout << "     Your Personal Emotional Check-in Tool    " << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << std::endl;
    std::cout << "EmpathyCLI helps you track your moods and offers" << std::endl;
    std::cout << "supportive resources when you need them most." << std::endl;
    std::cout << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.get();
    clearScreen();
}

// Function to display main menu
void displayMenu() {
    std::cout << "==============================================" << std::endl;
    std::cout << "                 MAIN MENU                    " << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "1. Log Your Current Mood" << std::endl;
    std::cout << "2. View Mood History" << std::endl;
    std::cout << "3. View Mood Statistics" << std::endl;
    std::cout << "4. Browse Support Resources" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << "==============================================" << std::endl;
}

// Function to add a new mood entry
void addNewMoodEntry(MoodTracker& tracker, const ResourceMap& resources) {
    clearScreen();
    std::cout << "==============================================" << std::endl;
    std::cout << "              LOG YOUR MOOD                   " << std::endl;
    std::cout << "==============================================" << std::endl;
    
    // Get mood score
    int score = 0;
    while (score < 1 || score > 10) {
        std::cout << "On a scale of 1-10, how would you rate your mood?" << std::endl;
        std::cout << "(1 = very low, 10 = excellent): ";
        
        if (!(std::cin >> score) || score < 1 || score > 10) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number between 1 and 10." << std::endl;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Get mood description
    std::string description;
    std::cout << "Describe how you're feeling in a few words: ";
    std::getline(std::cin, description);
    
    // Add to tracker
    tracker.addMoodEntry(score, description);
    
    // Get the entry we just added
    MoodEntry latestEntry = tracker.getLatestMood();
    
    // Display confirmation
    clearScreen();
    std::cout << "Mood logged successfully at " << formatTimestamp(latestEntry.timestamp) << std::endl;
    std::cout << "Score: " << latestEntry.score << "/10" << std::endl;
    std::cout << "Description: " << latestEntry.description << std::endl;
    std::cout << std::endl;
    
    // Show encouragement for all entries
    std::cout << getRandomEncouragement() << std::endl;
    std::cout << std::endl;
    
    // If mood score is low or has negative words, suggest resources
    if (score <= 4) {
        displayResourcesBasedOnMood(latestEntry, resources);
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

// Function to view mood history
void viewMoodHistory(const MoodTracker& tracker) {
    clearScreen();
    std::cout << "==============================================" << std::endl;
    std::cout << "              MOOD HISTORY                    " << std::endl;
    std::cout << "==============================================" << std::endl;
    
    const auto& history = tracker.getMoodHistory();
    
    if (history.empty()) {
        std::cout << "No mood entries found. Try logging your mood first." << std::endl;
    } else {
        std::cout << "Recent entries (most recent first):" << std::endl;
        std::cout << std::endl;
        
        // Display last 10 entries or all if fewer than 10
        int start = std::max(0, static_cast<int>(history.size()) - 10);
        for (int i = history.size() - 1; i >= start; --i) {
            const auto& entry = history[i];
            std::cout << "Date: " << formatTimestamp(entry.timestamp) << std::endl;
            std::cout << "Score: " << entry.score << "/10" << std::endl;
            std::cout << "Description: " << entry.description << std::endl;
            std::cout << "---------------------------------------------" << std::endl;
        }
        
        // If there are more entries than shown
        if (history.size() > 10) {
            std::cout << "... and " << history.size() - 10 << " more entries." << std::endl;
        }
    }
    
    std::cout << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

// Function to view mood statistics
void viewMoodStats(const MoodTracker& tracker) {
    clearScreen();
    std::cout << "==============================================" << std::endl;
    std::cout << "              MOOD STATISTICS                 " << std::endl;
    std::cout << "==============================================" << std::endl;
    
    const auto& history = tracker.getMoodHistory();
    
    if (history.empty()) {
        std::cout << "No mood entries found. Try logging your mood first." << std::endl;
    } else {
        // Calculate average score
        double average = tracker.getAverageMoodScore();
        
        // Find highest and lowest scores
        int highest = 0, lowest = 10;
        std::chrono::system_clock::time_point highestDate, lowestDate;
        
        for (const auto& entry : history) {
            if (entry.score > highest) {
                highest = entry.score;
                highestDate = entry.timestamp;
            }
            if (entry.score < lowest) {
                lowest = entry.score;
                lowestDate = entry.timestamp;
            }
        }
        
        // Display statistics
        std::cout << "Total entries: " << history.size() << std::endl;
        std::cout << "Average mood score: " << std::fixed << std::setprecision(1) << average << "/10" << std::endl;
        std::cout << "Highest mood: " << highest << "/10 on " << formatTimestamp(highestDate) << std::endl;
        std::cout << "Lowest mood: " << lowest << "/10 on " << formatTimestamp(lowestDate) << std::endl;
        
        // Display unique mood words if there are any
        const auto& uniqueWords = tracker.getUniqueMoodWords();
        if (!uniqueWords.empty()) {
            std::cout << std::endl;
            std::cout << "Words you've used to describe your moods:" << std::endl;
            
            // Print in a nice format
            int count = 0;
            for (const auto& word : uniqueWords) {
                std::cout << word;
                if (++count < uniqueWords.size()) {
                    std::cout << ", ";
                }
                
                // Add newline every 8 words for readability
                if (count % 8 == 0) {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

// Function to display all available resources
void displayResources(const ResourceMap& resources) {
    clearScreen();
    std::cout << "==============================================" << std::endl;
    std::cout << "              SUPPORT RESOURCES               " << std::endl;
    std::cout << "==============================================" << std::endl;
    
    // Get all mood-based resources
    auto allResources = resources.getAllMoodResources();
    
    if (allResources.empty()) {
        std::cout << "No resources available at this time." << std::endl;
    } else {
        std::cout << "Here are some resources that might help:" << std::endl;
        std::cout << std::endl;
        
        for (const auto& [mood, moodResources] : allResources) {
            std::cout << "For when you're feeling \"" << mood << "\":" << std::endl;
            
            for (const auto& resource : moodResources) {
                std::cout << "  - " << resource.title << std::endl;
                std::cout << "    " << resource.description << std::endl;
                std::cout << "    URL: " << resource.url << std::endl;
                std::cout << std::endl;
            }
        }
    }
    
    std::cout << std::endl;
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

// Function to clear the screen (cross-platform)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to confirm an action
bool confirmAction(const std::string& message) {
    std::cout << message;
    std::string response;
    std::getline(std::cin, response);
    
    // Convert to lowercase
    std::transform(response.begin(), response.end(), response.begin(),
                  [](unsigned char c){ return std::tolower(c); });
    
    return response == "y" || response == "yes";
}

// Function to display resources based on mood
void displayResourcesBasedOnMood(const MoodEntry& entry, const ResourceMap& resources) {
    std::cout << "Based on your mood, here are some resources that might help:" << std::endl;
    std::cout << std::endl;
    
    // Check for score-based resources first
    auto scoreResources = resources.getResourcesForScore(entry.score);
    
    // Extract words from the description for mood-based resources
    std::istringstream iss(entry.description);
    std::string word;
    std::vector<Resource> moodResources;
    
    while (iss >> word) {
        // Convert to lowercase
        std::transform(word.begin(), word.end(), word.begin(),
                      [](unsigned char c){ return std::tolower(c); });
        
        // Remove punctuation
        word.erase(std::remove_if(word.begin(), word.end(),
                                 [](unsigned char c){ return std::ispunct(c); }),
                  word.end());
        
        // Get resources for this word
        auto wordResources = resources.getResourcesForMood(word);
        
        // Add to our collection
        moodResources.insert(moodResources.end(), wordResources.begin(), wordResources.end());
    }
    
    // Combine both sets of resources
    std::vector<Resource> allResources = scoreResources;
    allResources.insert(allResources.end(), moodResources.begin(), moodResources.end());
    
    // Display resources or a message if none found
    if (allResources.empty()) {
        std::cout << "No specific resources found for your current mood." << std::endl;
        std::cout << "You can browse all available resources from the main menu." << std::endl;
    } else {
        // Display up to 3 resources to avoid overwhelming
        int count = 0;
        for (const auto& resource : allResources) {
            if (count >= 3) break;
            
            std::cout << count + 1 << ". " << resource.title << std::endl;
            std::cout << "   " << resource.description << std::endl;
            std::cout << "   URL: " << resource.url << std::endl;
            std::cout << std::endl;
            count++;
        }
        
        if (allResources.size() > 3) {
            std::cout << "... and " << allResources.size() - 3 << " more resources available from the main menu." << std::endl;
        }
    }
}

// Function to format timestamp for display
std::string formatTimestamp(const std::chrono::system_clock::time_point& timestamp) {
    auto timeT = std::chrono::system_clock::to_time_t(timestamp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Function to get a random encouraging message
std::string getRandomEncouragement() {
    static const std::vector<std::string> encouragements = {
        "Remember that feelings are temporary and will pass.",
        "Taking time to check in with yourself is an act of self-care.",
        "You're doing great by being mindful of your emotions.",
        "Every small step toward self-awareness matters.",
        "Your feelings are valid, whatever they may be.",
        "Being honest about your emotions takes courage.",
        "Self-reflection is a powerful tool for growth.",
        "You're not alone in how you feel.",
        "I appreciate you sharing how you're feeling today.",
        "Recognizing your emotions is the first step toward managing them."
    };
    
    // Random number generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, encouragements.size() - 1);
    
    return encouragements[distrib(gen)];
}
