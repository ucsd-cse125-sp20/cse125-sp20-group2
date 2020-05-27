#include <util/MapBuilder.h>

DungeonMap* MapBuilder::getBasicDungeonMap() {
    Wall* wall = new Wall();
    wall->setPosition(glm::vec3(0, 0, 5));
    DungeonMap *mp = new DungeonMap();
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

    mp->spawningLocations.push_back(glm::vec3(10, 0, 0));
    mp->spawningLocations.push_back(glm::vec3(-10, 0, 0));
    mp->spawningLocations.push_back(glm::vec3(0, 10, 0));
    mp->spawningLocations.push_back(glm::vec3(0, 0, 0));


    ///TODO: Add different positions?
    mp->ingredientPositions.push_back(glm::vec3(8, 0, 0));
    mp->ingredientPositions.push_back(glm::vec3(-8, 0, 0));
    mp->ingredientPositions.push_back(glm::vec3(8, 0, 0));
    mp->ingredientPositions.push_back(glm::vec3(-8, 0, 0));

    return mp;
}

KitchenMap* MapBuilder::getBasicKitchenMap() {
    Wall* wall = new Wall();
    wall->setPosition(glm::vec3(0, 0, 5));
    Map *mp = new KitchenMap();
    mp->wallList.push_back(wall);

    mp->spawningLocations.push_back(glm::vec3(10, 0, 0));
    mp->spawningLocations.push_back(glm::vec3(-10, 0, 0));
    mp->spawningLocations.push_back(glm::vec3(0, 10, 0));
    mp->spawningLocations.push_back(glm::vec3(0, 0, 0));

    CookwareObject* c = new CookwareObject();

    // TODO: Change to proper kitchenware object later
    c->setModel(Config::get("Bun_Model"))
    c->setPosition(glm::vec3(5, 0, 0))
    mp->cookwareObjects.push_back(c);

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