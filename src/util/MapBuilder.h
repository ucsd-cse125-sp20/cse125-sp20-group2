#pragma once

#include <objects/Wall.h>
#include <util/Recipe.h>
#include <objects/Map.h>
#include <objects/KitchenMap.h>
#include <objects/DungeonMap.h>
#include <objects/Table.h>
#include <objects/Plate.h>
#include <game/GameState.h>

//
class MapBuilder {
public:
    static DungeonMap* getBasicDungeonMap();
    static KitchenMap* getBasicKitchenMap(GameState* gameState);
    static void assignIngredientPositions( Recipe* recipe, DungeonMap* mp );
};