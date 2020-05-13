#pragma once

#include <objects/Wall.h>
#include <objects/Map.h>

class MapBuilder {
public:
    static Map* getBasicMap();
    static void assignIngredientPositions( Recipe* recipe, Map* mp )
};