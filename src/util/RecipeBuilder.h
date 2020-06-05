#include <util/Recipe.h>
#include <utility>
#include <util/Config.h>

class RecipeBuilder {
public:
    static Recipe* getBasicRecipe();
    static Recipe* getAnotherRecipe();
    static Ingredient* createIngredient(std::string modelPath);
};