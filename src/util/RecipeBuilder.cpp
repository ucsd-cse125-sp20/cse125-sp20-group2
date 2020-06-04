#include <util/RecipeBuilder.h>
#include <constants/gameplay_vars.h>

Recipe* RecipeBuilder::getBasicRecipe() {
    Recipe* recipe = new Recipe();
    recipe->name = "Glorious Communist Burger";

    Ingredient *ing = createIngredient(BUN);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(MEAT);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(ONION);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(CHEESE);
    recipe->ingredientList.push_back(ing);
    
    ing = createIngredient(LETTUCE);
    recipe->ingredientList.push_back(ing);
    
    ing = createIngredient(TOMATO);
    recipe->ingredientList.push_back(ing);

    int points = Config::getInt("Individual_Instruction_Points");

    Instruction *inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Cut;
    inst->points = points;
    inst->cookware = CUTTING_BOARD;
    inst->ingredient = MEAT;
    inst->instr = "Cut the meat";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Cut;
    inst->after = IngredientStatus::Fried;
    inst->points = points;
    inst->cookware = PAN;
    inst->ingredient = MEAT;
    inst->instr = "Fry the meat";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Boiled;
    inst->points = points;
    inst->cookware = POT;
    inst->ingredient = LETTUCE;
    inst->instr = "Boil the lettuce (because of e.coli)";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Cut;
    inst->points = points;
    inst->cookware = CUTTING_BOARD;
    inst->ingredient = CHEESE;
    inst->instr = "Cut the cheese";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Cut;
    inst->points = points;
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