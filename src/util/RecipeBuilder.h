#include <util/Recipe.h>
#include <utility>
#include <util/Config.h>

class RecipeBuilder {
public:
    static Recipe* getBasicRecipe();
    static Ingredient* createIngredient(std::string modelPath);
};