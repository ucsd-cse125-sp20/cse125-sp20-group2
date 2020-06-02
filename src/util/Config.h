#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

class Config {
public:

    // Config variables
    static std::unordered_map<std::string, std::string>* vars;

    /**
     * Get a config variable
     * */
    static std::string get(std::string key) {

        // Edge case: does not exist in config
        if (vars->find(key) == vars->end()) return std::string("NOT FOUND");

        // Return
        return vars->at(key);
    }

    static int getInt(std::string key) {
        return std::stoi(vars->at(key));
    }

    static float getFloat(std::string key) {
        return std::stof(vars->at(key));
    }

    /**
     * Parses a vec3 from the config file in the format
     * x,y,z .
     * */
    static glm::vec3 getVec3(std::string key) {

        // Get string key
        std::string str = vars->at(key);
        
        // Parse vector coordinate x
        int pos = str.find(',');
        float x = std::stof(str.substr(0, pos));
        str.erase(0, pos + 1);

        // Parse vector coordinate y
        pos = str.find(',');
        float y = std::stof(str.substr(0, pos));
        str.erase(0, pos + 1);

        // Parse remainder of string, assumed to be z
        float z = std::stof(str);

        // Return the full vector
        return glm::vec3(x, y, z);
    }

    /**
     * Parses a vec4 from the config file in the format
     * x,y,z,w .
     * */
    static glm::vec4 getVec4(std::string key) {

        // Get string key
        std::string str = vars->at(key);
        
        // Parse vector coordinate x
        int pos = str.find(',');
        float x = std::stof(str.substr(0, pos));
        str.erase(0, pos + 1);

        // Parse vector coordinate y
        pos = str.find(',');
        float y = std::stof(str.substr(0, pos));
        str.erase(0, pos + 1);

        // Parse vector coordinate z
        pos = str.find(',');
        float z = std::stof(str.substr(0, pos));
        str.erase(0, pos + 1);

        // Parse remainder of the string, assumed to be w
        float w = std::stof(str);

        // Return the full vector
        return glm::vec4(x, y, z, w);
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