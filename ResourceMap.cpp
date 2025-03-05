#include "ResourceMap.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

// Resource constructor
Resource::Resource(const std::string& title, const std::string& url, const std::string& description)
    : title(title), url(url), description(description) {}

// ResourceMap constructor
ResourceMap::ResourceMap() {
    initializeDefaultResources();
}

// Initialize with default resources
void ResourceMap::initializeDefaultResources() {
    // Add default resources for common negative mood words
    addResourceForMood("sad", Resource("5-Minute Meditation", 
                                      "https://www.youtube.com/watch?v=inpok4MKVLM",
                                      "A short meditation to help center yourself"));
    
    addResourceForMood("anxious", Resource("Deep Breathing Exercise", 
                                          "https://www.health.harvard.edu/lung-health-and-disease/learning-diaphragmatic-breathing",
                                          "Learn how to practice deep diaphragmatic breathing"));
    
    addResourceForMood("stressed", Resource("Progressive Muscle Relaxation", 
                                           "https://www.healthline.com/health/progressive-muscle-relaxation",
                                           "A technique to relax your body and mind"));
    
    addResourceForMood("angry", Resource("Anger Management Techniques", 
                                        "https://www.mayoclinic.org/healthy-lifestyle/adult-health/in-depth/anger-management/art-20045434",
                                        "Healthy ways to express and deal with anger"));
    
    addResourceForMood("tired", Resource("Energy-Boosting Tips", 
                                        "https://www.health.harvard.edu/healthbeat/9-tips-to-boost-your-energy-naturally",
                                        "Natural ways to increase your energy levels"));
    
    // Add resources for low mood scores (1-3)
    addResourceForScore(1, Resource("Crisis Text Line", 
                                   "https://www.crisistextline.org/",
                                   "Text HOME to 741741 to connect with a Crisis Counselor"));
    
    addResourceForScore(2, Resource("7 Ways to Boost Your Mood", 
                                   "https://www.psychologytoday.com/us/blog/click-here-happiness/201801/how-boost-your-mood-7-ways-feeling-happier-every-day",
                                   "Quick strategies to improve your mood"));
    
    addResourceForScore(3, Resource("Self-Compassion Exercise", 
                                   "https://self-compassion.org/exercise-2-self-compassion-break/",
                                   "A brief exercise to be kinder to yourself"));
}

// Load resources from a JSON file
bool ResourceMap::loadResourcesFromFile(const std::string& filename) {
    try {
        // Open the file
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            return false;
        }
        
        // Parse JSON
        json resourcesJson;
        inFile >> resourcesJson;
        
        // Clear existing resources
        moodToResources.clear();
        scoreToResources.clear();
        
        // Load mood-based resources
        if (resourcesJson.contains("moodResources")) {
            for (const auto& [mood, resources] : resourcesJson["moodResources"].items()) {
                for (const auto& res : resources) {
                    std::string title = res["title"];
                    std::string url = res["url"];
                    std::string description = res["description"];
                    
                    addResourceForMood(mood, Resource(title, url, description));
                }
            }
        }
        
        // Load score-based resources
        if (resourcesJson.contains("scoreResources")) {
            for (const auto& [scoreStr, resources] : resourcesJson["scoreResources"].items()) {
                int score = std::stoi(scoreStr);
                
                for (const auto& res : resources) {
                    std::string title = res["title"];
                    std::string url = res["url"];
                    std::string description = res["description"];
                    
                    addResourceForScore(score, Resource(title, url, description));
                }
            }
        }
        
        return true;
    } catch (...) {
        // Keep default resources in case of errors
        return false;
    }
}

// Add a resource for a specific mood keyword
void ResourceMap::addResourceForMood(const std::string& mood, const Resource& resource) {
    // Convert mood to lowercase for consistency
    std::string moodLower = mood;
    std::transform(moodLower.begin(), moodLower.end(), moodLower.begin(),
                  [](unsigned char c){ return std::tolower(c); });
    
    moodToResources[moodLower].push_back(resource);
}

// Add a resource for a specific mood score
void ResourceMap::addResourceForScore(int score, const Resource& resource) {
    // Ensure score is in valid range (1-10)
    if (score >= 1 && score <= 10) {
        scoreToResources[score].push_back(resource);
    }
}

// Get resources based on mood description
std::vector<Resource> ResourceMap::getResourcesForMood(const std::string& mood) const {
    // Convert mood to lowercase
    std::string moodLower = mood;
    std::transform(moodLower.begin(), moodLower.end(), moodLower.begin(),
                  [](unsigned char c){ return std::tolower(c); });
    
    // Look for exact match
    auto it = moodToResources.find(moodLower);
    if (it != moodToResources.end()) {
        return it->second;
    }
    
    // If no exact match, return empty vector
    return {};
}

// Get resources based on mood score
std::vector<Resource> ResourceMap::getResourcesForScore(int score) const {
    auto it = scoreToResources.find(score);
    if (it != scoreToResources.end()) {
        return it->second;
    }
    
    return {};
}

// Get all available resources
std::map<std::string, std::vector<Resource>> ResourceMap::getAllMoodResources() const {
    return moodToResources;
}
