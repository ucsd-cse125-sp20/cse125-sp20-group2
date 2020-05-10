#pragma once

#include <util/Instruction.h>

class Recipe {
public:
    std::string name;
    std::vector<Instruction*> instructionList;
    std::vector<IngredientObject*> ingredientList;
};