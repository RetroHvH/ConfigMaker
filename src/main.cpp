#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <conio.h>
#include <string>

using json = nlohmann::json; // Don't have to do this, vcpkg

// ConfigManager: A class dedicated to managing the config of our cheat.
class ConfigManager {
public:
    explicit ConfigManager(const std::string& fileName)
        : configFile(fileName) {
    }

    // Save the config data to a file in a well-structured JSON format.
    bool saveConfig(const json& configData) {
        try {
            std::ofstream outFile(configFile, std::ios::trunc);
            if (!outFile) {
                throw std::ios_base::failure("Unable to open config file for writing.");
            }
            outFile << configData.dump(4); // Pretty-printing the JSON with an indentation of 4 spaces.
            outFile.close();
            std::cout << "[INFO] Config successfully saved to '" << configFile << "'\n";
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to save config: " << e.what() << "\n";
            return false;
        }
    }

    // Load the config data from a file, with error handling for invalid files.
    bool loadConfig(json& configData) {
        try {
            std::ifstream inFile(configFile);
            if (!inFile) {
                throw std::ios_base::failure("Config file not found or unable to open.");
            }
            inFile >> configData;
            inFile.close();
            std::cout << "[INFO] Config successfully loaded from '" << configFile << "'\n";
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to load config: " << e.what() << "\n";
            return false;
        }
    }

private:
    std::string configFile; // Path to the config file.
};

// Function to create a default config for the cheat.
json createDefaultConfig() {
    return {
        {"aimbot", {
            {"enabled", true},
            {"fov", 90.0},
            {"smoothness", 0.5}
        }},
        {"esp", {
            {"enabled", true},
            {"color", {255, 0, 0}},
            {"distance", 500}
        }},
        {"misc", {
            {"sound", true},
            {"visuals", true}
        }}
    };
}

int main() {
    ConfigManager configManager("config.json");

    // Default cheat config to be used if no saved config exists.
    json defaultConfig = createDefaultConfig();

    // Attempt to load the previous config or fall back to the default.
    json loadedConfig;
    if (!configManager.loadConfig(loadedConfig)) {
        std::cout << "[INFO] Using default config as no previous config was found.\n";
        loadedConfig = defaultConfig; // Fall back to the default config.
    }

    // Display the loaded (or default) config.
    std::cout << "\n[CURRENT CONFIG]:\n" << loadedConfig.dump(4) << "\n";

    // Main loop to handle user input for saving or loading the config.
    while (true) {
        std::cout << "\nPress F5 to save the current config, F6 to load a new config.\n";
        int key = _getch();  // Wait for a key press.

        if (key == 0 || key == 224) {  // Special keys like F5/F6 are read as 0 or 224.
            key = _getch();  // Get the actual key code.

            if (key == 0x3F) {  // F5 key
                std::cout << "[INFO] Saving config...\n";
                if (!configManager.saveConfig(loadedConfig)) {
                    std::cerr << "[ERROR] Unable to save config.\n";
                }
            }
            else if (key == 0x40) {  // F6 key
                std::cout << "[INFO] Loading new config...\n";
                if (!configManager.loadConfig(loadedConfig)) {
                    std::cerr << "[ERROR] Failed to load config.\n";
                }
                std::cout << "\n[NEW CONFIG LOADED]:\n" << loadedConfig.dump(4) << "\n";
            }
        }
    }

    return 0;
}