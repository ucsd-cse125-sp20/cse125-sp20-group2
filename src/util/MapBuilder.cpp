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
    mp->ingredientPositions.push_back(glm::vec3(10, 0, 0));
    mp->ingredientPositions.push_back(glm::vec3(-10, 0, 0));

    return mp;
}

KitchenMap* MapBuilder::getBasicKitchenMap() {
    Wall* wall = new Wall();
    wall->setPosition(glm::vec3(0, 0, 5));
    KitchenMap* mp = new KitchenMap();
    mp->wallList.push_back(wall);

    mp->spawningLocations.push_back(glm::vec3(10, 0, 0));
    mp->spawningLocations.push_back(glm::vec3(-10, 0, 0));
    mp->spawningLocations.push_back(glm::vec3(0, 10, 0));
    mp->spawningLocations.push_back(glm::vec3(0, 0, 0));

    Cookware* c = new Cookware();

    c->setModel(Config::get("Pan_Model"));
    c->setName(PAN);
    c->setPosition(glm::vec3(3, 0, 0));
    
    Table *t = new Table();
    t->setPosition(glm::vec3(5, 0, 0));
    mp->tableList.push_back(t);

    mp->cookwareObjects.push_back(c);

    return mp;
}

void MapBuilder::assignIngredientPositions( Recipe* recipe, DungeonMap* mp ) {
    /*int i = 0;
    for( auto ingredient : recipe->ingredientList ) {
        ingredient->setPosition(mp->ingredientPositions[i]);
        i++;
    }*/

    DungeonMap* dMapPtr = static_cast<DungeonMap*>(mp);
    for (int i = 0; i < recipe->ingredientList.size(); i++) {
        Ingredient* ing = recipe->ingredientList.front();
        recipe->ingredientList.erase(recipe->ingredientList.begin());
        ing->setPosition(dMapPtr->ingredientPositions[i]);
        recipe->ingredientList.push_back(ing);
    }
}