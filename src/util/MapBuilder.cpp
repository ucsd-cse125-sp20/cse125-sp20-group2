#include <util/MapBuilder.h>

DungeonMap* MapBuilder::getBasicDungeonMap() {

    // Create the dungeon map
    DungeonMap *mp = new DungeonMap();

    // Get wall count
    int dungeonWallCount = Config::getInt("Dungeon_Wall_Count");
    
    // Iterate over walls and add them to the map
    for (int i = 1; i <= dungeonWallCount; i++)
    {
        Wall* wall = new Wall();
        wall->setPosition(Config::getVec3("Dungeon_Wall_Pos_" + std::to_string(i)));
        
        // Get scale and rotation
        std::string scaleVecString = Config::get("Dungeon_Wall_Scale_" + std::to_string(i));
        std::string rotString = Config::get("Dungeon_Wall_Rot_" + std::to_string(i));

        // Apply scale and rotation, if applicable
        if (scaleVecString.compare(std::string("NOT FOUND")) != 0)
        {
            glm::vec3 scaleVec = Config::getVec3("Dungeon_Wall_Scale_" + std::to_string(i));
            wall->applyScale(scaleVec);
        }
        if (rotString.compare(std::string("NOT FOUND")) != 0)
        {
            float rot = Config::getFloat("Dungeon_Wall_Rot_" + std::to_string(i));
            wall->setRotation(rot);
        }

        // Add wall to list
        mp->wallList.push_back(wall);
    }

    // Get spawn count
    int spawnCount = Config::getInt("Dungeon_Spawn_Count");

    // Iterate over spawns and add to map
    for (int i = 1; i <= spawnCount; i++)
    {
        mp->spawningLocations.push_back(Config::getVec3("Dungeon_Spawn_" + std::to_string(i)));
    }

    // Set box for ingredient spawning position
    mp->lowerX = Config::getFloat("Dungeon_Lower_X");
    mp->upperX = Config::getFloat("Dungeon_Upper_X");
    mp->lowerZ = Config::getFloat("Dungeon_Lower_Z");
    mp->upperZ = Config::getFloat("Dungeon_Upper_Z");

    // Set time difference between spawning ingredients
    mp->timeDelta = std::chrono::seconds(Config::getInt("Dungeon_Ingredient_Spawn_Rate"));

    // Set initial time to spawn ingredient
    mp->ingredientSpawnTime = std::chrono::high_resolution_clock::now();

    return mp;
}

KitchenMap* MapBuilder::getBasicKitchenMap(GameState* gameState) {
    Wall* wall = new Wall();
    wall->setPosition(glm::vec3(0, 0, 5));
    KitchenMap* mp = new KitchenMap();
    mp->wallList.push_back(wall);

    mp->spawningLocations.push_back(glm::vec3(10, 0, 0));
    mp->spawningLocations.push_back(glm::vec3(-10, 0, 0));
    mp->spawningLocations.push_back(glm::vec3(0, 0, -5));
    mp->spawningLocations.push_back(glm::vec3(0, 0, 0));

    // Add cookware
    Cookware* c = new Cookware(PAN);
    c->setName(PAN);
    c->setPosition(glm::vec3(3, 0, 0));
    mp->cookwareObjects.push_back(c);

    // Add cookware
    c = new Cookware(CUTTING_BOARD);
    c->setName(PAN);
    c->setPosition(glm::vec3(-3, 0, 0));
    mp->cookwareObjects.push_back(c);

    // Add cookware
    c = new Cookware(POT);
    c->setName(PAN);
    c->setPosition(glm::vec3(0, 0, -3));
    mp->cookwareObjects.push_back(c);

    // Add plate
    Plate* p = new Plate();
    p->setPosition(glm::vec3(7, 0, 0));
    mp->plateObjects.push_back(p);

    return mp;
}

void MapBuilder::assignIngredientPositions( Recipe* recipe, DungeonMap* mp ) {
    /*int i = 0;
    for( auto ingredient : recipe->ingredientList ) {
        ingredient->setPosition(mp->ingredientPositions[i]);
        i++;
    }*/
    std::cout << "WARNING, INGREDIENT POSITIONS ARE NOT BEING ASSIGNED" << std::endl;
    // DungeonMap* dMapPtr = static_cast<DungeonMap*>(mp);
    // for (int i = 0; i < recipe->ingredientList.size(); i++) {
    //     Ingredient* ing = recipe->ingredientList.front();
    //     recipe->ingredientList.erase(recipe->ingredientList.begin());
    //     ing->setPosition(dMapPtr->ingredientPositions[i]);
    //     recipe->ingredientList.push_back(ing);
    // }
}