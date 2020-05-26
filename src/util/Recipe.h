#pragma once

#include <util/Instruction.h>
#include <queue>

class Recipe {
public:
    std::string name;
    std::vector<Instruction*> instructionList;
    std::queue<IngredientObject*> ingredientList;
};