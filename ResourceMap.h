#ifndef RESOURCE_MAP_H
#define RESOURCE_MAP_H

#include <map>
#include <string>
#include <vector>

// Structure to store resource information
struct Resource {
    std::string title;           // Title of the resource
    std::string url;             // URL to the resource
    std::string description;     // Brief description of what it offers
    
    // Constructor for easy creation
    Resource(const std::string& title, const std::string& url, const std::string& description);
};

class ResourceMap {
private:
    // Map mood keywords to relevant resources
    std::map<std::string, std::vector<Resource>> moodToResources;
    
    // Map mood score ranges to relevant resources
    std::map<int, std::vector<Resource>> scoreToResources;

public:
    ResourceMap();
    
    // Initialize with default resources
    void initializeDefaultResources();
    
    // Load resources from a JSON file
    bool loadResourcesFromFile(const std::string& filename);
    
    // Add a resource for a specific mood keyword
    void addResourceForMood(const std::string& mood, const Resource& resource);
    
    // Add a resource for a specific mood score
    void addResourceForScore(int score, const Resource& resource);
    
    // Get resources based on mood description
    std::vector<Resource> getResourcesForMood(const std::string& mood) const;
    
    // Get resources based on mood score
    std::vector<Resource> getResourcesForScore(int score) const;
    
    // Get all available resources
    std::map<std::string, std::vector<Resource>> getAllMoodResources() const;
};

#endif // RESOURCE_MAP_H
