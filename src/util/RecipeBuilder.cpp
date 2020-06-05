#include <util/RecipeBuilder.h>
#include <constants/gameplay_vars.h>

Recipe* RecipeBuilder::getAnotherRecipe() {
    Recipe* recipe = new Recipe();
    recipe->name = "Glorious Communist Cheeseburger";

    Ingredient *ing = createIngredient(BUN);
    ing->setStatus(IngredientStatus::Raw);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(CHEESE);
    ing->setStatus(IngredientStatus::Cut);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(MEAT);
    ing->setStatus(IngredientStatus::Fried);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(ONION);
    ing->setStatus(IngredientStatus::Cut);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(CHEESE);
    ing->setStatus(IngredientStatus::Cut);
    recipe->ingredientList.push_back(ing);
    
    ing = createIngredient(LETTUCE);
    ing->setStatus(IngredientStatus::Boiled);
    recipe->ingredientList.push_back(ing);
    
    ing = createIngredient(TOMATO);
    ing->setStatus(IngredientStatus::Cut);
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
    inst->instr = "Cut the first block of cheese";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Cut;
    inst->points = points;
    inst->cookware = CUTTING_BOARD;
    inst->ingredient = CHEESE;
    inst->instr = "Cut the second block of cheese";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Raw;
    inst->after = IngredientStatus::Cut;
    inst->points = points;
    inst->cookware = CUTTING_BOARD;
    inst->ingredient = ONION;
    inst->instr = "Cut the onion";
    recipe->instructionList.push_back(inst);

    inst = new Instruction();
    inst->before = IngredientStatus::Cut;
    inst->after = IngredientStatus::Fried;
    inst->points = points;
    inst->cookware = PAN;
    inst->ingredient = ONION;
    inst->instr = "Fry the onion";
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

Recipe* RecipeBuilder::getBasicRecipe() {
    Recipe* recipe = new Recipe();
    recipe->name = "Glorious Communist Burger";

    Ingredient *ing = createIngredient(BUN);
    ing->setStatus(IngredientStatus::Raw);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(MEAT);
    ing->setStatus(IngredientStatus::Fried);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(ONION);
    ing->setStatus(IngredientStatus::Cut);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(CHEESE);
    ing->setStatus(IngredientStatus::Cut);
    recipe->ingredientList.push_back(ing);
    
    ing = createIngredient(LETTUCE);
    ing->setStatus(IngredientStatus::Boiled);
    recipe->ingredientList.push_back(ing);
    
    ing = createIngredient(TOMATO);
    ing->setStatus(IngredientStatus::Cut);
    recipe->ingredientList.push_back(ing);

    ing = createIngredient(BUN);
    ing->setStatus(IngredientStatus::Raw);
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
    inst->ingredient = ONION;
    inst->instr = "Cut the onion";
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