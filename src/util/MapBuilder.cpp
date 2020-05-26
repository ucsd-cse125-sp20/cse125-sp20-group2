#include <util/MapBuilder.h>

Map* MapBuilder::getBasicMap() {
    Wall* wall = new Wall();
    wall->setPosition(glm::vec3(0, 0, 5));
    Map *mp = new Map();
    mp->wallList.push_back(wall);

    wall = new Wall();
    wall->setPosition(glm::vec3(0, 0, -5));
    mp->wallList.push_back(wall);

    wall = new Wall();
    wall->setPosition(glm::vec3(5, 0, 0));
    mp->wallList.push_back(wall);

    wall = new Wall();
    wall->setPosition(glm::vec3(-5, 0, 0));
    mp->wallList.push_back(wall);

    mp->spawningLocations.push_back(glm::vec3(-1, 0, 0));
    mp->spawningLocations.push_back(glm::vec3(1, 0, 0));

    ///TODO: Add different positions?
    mp->ingredientPositions.push_back(glm::vec3(8, 0, 0));
    mp->ingredientPositions.push_back(glm::vec3(-8, 0, 0));
    mp->ingredientPositions.push_back(glm::vec3(8, 0, 0));
    mp->ingredientPositions.push_back(glm::vec3(-8, 0, 0));

    return mp;
}

void MapBuilder::assignIngredientPositions( Recipe* recipe, Map* mp ) {
    /*int i = 0;
    for( auto ingredient : recipe->ingredientList ) {
        ingredient->setPosition(mp->ingredientPositions[i]);
        i++;
    }*/
    for (int i = 0; i < recipe->ingredientList.size(); i++) {
        IngredientObject* ing = recipe->ingredientList.front();
        recipe->ingredientList.pop();
        ing->setPosition(mp->ingredientPositions[i]);
        recipe->ingredientList.push(ing);
    }
}