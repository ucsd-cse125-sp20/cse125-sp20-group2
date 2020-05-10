#pragma once

#include <objects/Wall.h>
#include <objects/IngredientObject.h>

class Map {
public:
    std::vector<Wall*> wallList;
    std::vector<IngredientObject*> ingredients;
};