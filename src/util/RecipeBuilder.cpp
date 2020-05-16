#include <util/RecipeBuilder.h>

Recipe* RecipeBuilder::getBasicRecipe() {
    Recipe* recipe = new Recipe();
    recipe->name = "Bun Burger";

    IngredientObject *ing = createIngredient("Bun_Model");
    recipe->ingredientList.push_back(ing);

    ing = createIngredient("Bun_Model");
    recipe->ingredientList.push_back(ing);

    return recipe;
}

IngredientObject* RecipeBuilder::createIngredient( std::string modelPath ) {
    IngredientObject* obj = new IngredientObject();
    obj->setModel(Config::get(modelPath));
    obj->getBoundingBox()->setRadius(1);
    return obj;
}