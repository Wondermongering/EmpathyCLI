cmake_minimum_required(VERSION 3.10)
project(EmpathyCLI VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include nlohmann/json for JSON handling
include(FetchContent)
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.cpp
    src/MoodTracker.cpp
    src/ResourceMap.cpp
)

# Add executable
add_executable(EmpathyCLI ${SOURCES})

# Link libraries
target_link_libraries(EmpathyCLI PRIVATE nlohmann_json::nlohmann_json)

# Copy resources to build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_BINARY_DIR}/resources/)
