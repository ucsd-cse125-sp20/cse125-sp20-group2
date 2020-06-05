#pragma once

#include <objects/Wall.h>
#include <objects/Ingredient.h>

class Map {
    public:
        // Walls include tables
        std::vector<Wall*> wallList;
        std::vector<glm::vec3> spawningLocations;
};