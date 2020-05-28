#pragma once

#include <objects/Ingredient.h>
#include <objects/Cookware.h>
#include <string>

class Instruction {
public:
    Ingredient* ingredient;
    Cookware* cookware;
    IngredientStatus before;
    IngredientStatus after;
    std::string instr;
    int points;
};