#pragma once

#include <objects/IngredientObject.h>
#include <util/Instruction.h>

class Recipe {
public:
    string name;
    vector<Instruction*> instructionList;
    vector<IngredientObject*> ingredientList;
}