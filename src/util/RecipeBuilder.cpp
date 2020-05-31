#include <util/RecipeBuilder.h>

Recipe* RecipeBuilder::getBasicRecipe() {
    Recipe* recipe = new Recipe();
    recipe->name = "Bun Burger";

    Ingredient *ing = createIngredient("Bun");
    recipe->ingredientList.push_back(ing);

    ing = createIngredient("Meat");
    recipe->ingredientList.push_back(ing);

    ing = createIngredient("Onion");
    recipe->ingredientList.push_back(ing);

    ing = createIngredient("Cheese");
    recipe->ingredientList.push_back(ing);
    
    ing = createIngredient("Lettuce");
    recipe->ingredientList.push_back(ing);
    
    ing = createIngredient("Tomato");
    recipe->ingredientList.push_back(ing);

    Instruction *inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Fried;
    inst->points = 10;
    inst->cookware = PAN;
    inst->ingredient = BUN;
    inst->instr = "Fry the meat";
    recipe->instructionList.push_back(inst);

    return recipe;
}

Ingredient* RecipeBuilder::createIngredient( std::string model ) {
    Ingredient* obj = new Ingredient();
    obj->setName(model);
    obj->setModel(Config::get(model + "_Model"));
    obj->getBoundingBox()->setRadius(1);
    return obj;
}