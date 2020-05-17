#pragma once

#include <objects/IngredientObject.h>
#include <objects/CookwareObject.h>
#include <string>

class Instruction {
public:
    IngredientObject* ingredient;
    CookwareObject* cookware;
    std::string instr;
    int points;
};