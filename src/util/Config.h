#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

class Config {
public:

    // Config variables
    static std::unordered_map<std::string, std::string>* vars;

    /**
     * Get a config variable
     * */
    static std::string get(std::string key) {
        return vars->at(key);
    }

    /**
     * Load config variables.
     * */
    static void load() 
    {
        // Get known config path
        std::ifstream configFile("config/config.txt");

        // Full line of text to be read
        std::string line;

        // Read lines of the config file
        while (std::getline(configFile, line))
        {
            // Skip comments and empty lines
            if (line.length() == 0 || line.at(0) == '/') continue;

            // Read string
            std::istringstream lineStream(line);

            // Key/value pair to be extracted
            std::string key, value;

            // Get next key/value pair
            std::getline(lineStream, key, '=');
            std::getline(lineStream, value);

            // Store data
            (*vars)[key] = value;
        }
    }

    /**
     * Deallocate map of config variables.
     * */
    static void unload()
    {
        delete vars;
    }
};