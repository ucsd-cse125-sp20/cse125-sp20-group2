#include <IngredientObject.h>

class Recipe {
public:
    string name;
    std::deque<Instruction> instructionList;
}