#pragma once

#include <objects/Wall.h>
#include <util/Recipe.h>
#include <objects/Map.h>
#include <objects/KitchenMap.h>
#include <objects/DungeonMap.h>

//
class MapBuilder {
public:
    static DungeonMap* getBasicDungeonMap();
    static KitchenMap* getBasicKitchenMap();
    static void assignIngredientPositions( Recipe* recipe, DungeonMap* mp );
};