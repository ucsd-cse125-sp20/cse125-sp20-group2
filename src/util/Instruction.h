#pragma once

#include <objects/Ingredient.h>
#include <objects/Cookware.h>
#include <string>

class Instruction {
public:
    std::string ingredient;
    std::string cookware;
    IngredientStatus before;
    IngredientStatus after;
    std::string instr;
    int points;
};