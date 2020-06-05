#pragma once

#include <objects/Map.h>
#include <chrono>

class DungeonMap : public Map {
    public:
        // Used to define the edges of the map, to spawn ingredients
        int lowerX;
        int upperX;
        int lowerZ;
        int upperZ;

        // Used to determine the next spawn time of the ingredient
        std::chrono::high_resolution_clock::time_point ingredientSpawnTime;

        // Used to determine how long before next ingredient spawned
        std::chrono::seconds timeDelta;

        // Used to render the map invisible
        void setRender(bool b);
};