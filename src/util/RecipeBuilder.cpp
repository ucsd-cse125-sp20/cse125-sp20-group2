#include <util/RecipeBuilder.h>

Recipe* RecipeBuilder::getBasicRecipe() {
    Recipe* recipe = new Recipe();
    recipe->name = "Bun Burger";

    IngredientObject *ing = createIngredient("Bun");
    recipe->ingredientList.push(ing);

    ///TODO: add more ingredients
    ing = createIngredient("Bun");
    recipe->ingredientList.push(ing);

    ing = createIngredient("Meat");
    recipe->ingredientList.push(ing);

    ing = createIngredient("Onion");
    recipe->ingredientList.push(ing);

    return recipe;
}

IngredientObject* RecipeBuilder::createIngredient( std::string model ) {
    IngredientObject* obj = new IngredientObject();
    obj->setName(model);
    obj->setModel(Config::get(model + "_Model"));
    obj->getBoundingBox()->setRadius(1);
    return obj;
}