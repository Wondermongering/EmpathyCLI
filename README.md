# EmpathyCLI

A compassionate command-line tool for tracking emotional well-being and providing helpful resources during difficult moments.

## Overview

EmpathyCLI is a C++ application that allows you to log and monitor your daily emotional states. It stores your mood data over time and intelligently suggests helpful resources based on your emotional needs. Whether you're feeling stressed, anxious, or just want to maintain awareness of your emotional patterns, EmpathyCLI provides a simple yet powerful interface for emotional self-care.

## Features

- **Mood Logging**: Record your emotional state with both numerical scores (1-10) and descriptive text
- **Mood History**: View your past entries to identify patterns and track your emotional journey
- **Mood Statistics**: Get insights into your emotional trends, including average mood scores and frequently used mood words
- **Resource Suggestions**: Receive targeted recommendations for helpful resources based on your current emotional state
- **Resource Library**: Browse a curated collection of support resources for various emotional states
- **Data Persistence**: Your mood history is saved between sessions in JSON format

## Getting Started

### Prerequisites

- C++17 compatible compiler
- CMake (version 3.10 or higher)
- Internet connection (to access suggested resources)

### Building from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/EmpathyCLI.git
   cd EmpathyCLI
   ```

2. Create a build directory and build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. Run the application:
   ```bash
   ./EmpathyCLI
   ```

## Usage Guide

### Logging Your Mood

1. Select option `1` from the main menu
2. Enter a mood score from 1-10 (where 1 is very low and 10 is excellent)
3. Provide a brief description of how you're feeling
4. If your mood score is low (≤4), the system will suggest helpful resources

### Viewing Your Mood History

1. Select option `2` from the main menu
2. Browse through your recent mood entries, with timestamps and descriptions

### Checking Mood Statistics

1. Select option `3` from the main menu
2. View aggregated data including:
   - Average mood score
   - Highest and lowest recorded moods with dates
   - Collection of unique words you've used to describe your feelings

### Browsing Support Resources

1. Select option `4` from the main menu
2. Browse through categorized resources for different emotional states

## Data Storage

Your mood data is stored locally in `data/mood_history.json`. No data is sent to external servers, ensuring your emotional journey remains private.

## Customizing Resources

You can add your own resources by editing the `resources/empathylinks.json` file. The format is:

```json
{
  "moodResources": {
    "sad": [
      {
        "title": "5-Minute Meditation",
        "url": "https://example.com/meditation",
        "description": "A short meditation to help center yourself"
      }
    ]
  },
  "scoreResources": {
    "3": [
      {
        "title": "Self-Compassion Exercise",
        "url": "https://example.com/self-compassion",
        "description": "A brief exercise to be kinder to yourself"
      }
    ]
  }
}
```

## Project Structure

```
EmpathyCLI/
├─ CMakeLists.txt
├─ README.md
├─ src/
│  ├─ main.cpp          # CLI interface and main program logic
│  ├─ MoodTracker.cpp   # Handles storing and analyzing mood entries
│  ├─ MoodTracker.h
│  ├─ ResourceMap.cpp   # Maps moods to helpful resources
│  ├─ ResourceMap.h
├─ resources/
│  └─ empathylinks.json # Resource mapping configuration
├─ data/                # Created at runtime to store mood history
   └─ mood_history.json
```

## Why It Helps

Emotional awareness is a key component of mental well-being. By regularly checking in with yourself and recording your emotional state, you can:

- Identify patterns in your mood fluctuations
- Recognize triggers that affect your emotional state
- Access relevant resources when you need them most
- Build a more mindful relationship with your emotions

## Contributing

Contributions are welcome! If you have ideas for new features, resources, or improvements, please feel free to:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by the importance of emotional self-care and awareness
- Built with compassion and empathy in mind
