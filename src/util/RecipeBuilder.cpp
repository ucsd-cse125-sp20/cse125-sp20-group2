#include <util/RecipeBuilder.h>

Recipe* RecipeBuilder::getBasicRecipe() {
    Recipe* recipe = new Recipe();
    recipe->name = "Glorious Communist Burger";

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
    inst->after = IngredientStatus::Cut;
    inst->points = 5;
    inst->cookware = CUTTING_BOARD;
    inst->ingredient = MEAT;
    inst->instr = "Cut the meat";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Cut;
    inst->after = IngredientStatus::Fried;
    inst->points = 10;
    inst->cookware = PAN;
    inst->ingredient = MEAT;
    inst->instr = "Fry the meat";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Boiled;
    inst->points = 10;
    inst->cookware = POT;
    inst->ingredient = LETTUCE;
    inst->instr = "Boil the lettuce (because of e.coli)";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Cut;
    inst->points = 5;
    inst->cookware = CUTTING_BOARD;
    inst->ingredient = CHEESE;
    inst->instr = "Cut the cheese";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Cut;
    inst->points = 5;
    inst->cookware = CUTTING_BOARD;
    inst->ingredient = TOMATO;
    inst->instr = "Cut the tomato";
    recipe->instructionList.push_back(inst);

    return recipe;
}

Ingredient* RecipeBuilder::createIngredient( std::string model ) {
    Ingredient* obj = new Ingredient();
    obj->setName(model);
    obj->setModel(Config::get(model + "_Model"));
    obj->getBoundingBox()->setRadius(1);
    obj->applyScale(Config::getVec3("Ingredient_Scaling"));
    return obj;
}