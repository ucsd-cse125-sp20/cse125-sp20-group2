#include<util/Instruction.h>

class CookEvent {
public:
    Ingredient* ingredient;
    Cookware* cookware;
    Player* player;
    IngredientStatus before;
    IngredientStatus after;
    std::chrono::high_resolution_clock::time_point terminationTime;
};