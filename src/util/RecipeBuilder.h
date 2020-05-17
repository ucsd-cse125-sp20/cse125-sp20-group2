#include <util/Recipe.h>
#include <utility>
#include <util/Config.h>

class RecipeBuilder {
public:
    static Recipe* getBasicRecipe();
    static IngredientObject* createIngredient(std::string modelPath);
};