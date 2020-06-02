#include <util/MapBuilder.h>

DungeonMap* MapBuilder::getBasicDungeonMap() {

    // Create the dungeon map
    DungeonMap *mp = new DungeonMap();

    // Get wall count
    int dungeonWallCount = Config::getInt("Dungeon_Wall_Count");
    
    // Iterate over walls and add them to the map
    for (int i = 0; i < dungeonWallCount; i++)
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
    for (int i = 0; i < spawnCount; i++)
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

    // Get spawn count
    int spawnCount = Config::getInt("Kitchen_Spawn_Count");

    // Iterate over spawns and add to map
    for (int i = 0; i < spawnCount; i++)
    {
        mp->spawningLocations.push_back(Config::getVec3("Kitchen_Spawn_" + std::to_string(i)));
    }

    // Add plates (vary by x)
    for (int i = 0; i < spawnCount; i++)
    {
        glm::vec3 currPos = Config::getVec3("Plate_Init_Spawn");
        float variation = Config::getFloat("Plate_Spacing");
        currPos.x += i * variation;
        Plate* plate = new Plate();
        plate->setPosition(currPos);
        mp->plateObjects.push_back(plate);
    }

    // Add cutting boards (vary by x)
    for (int i = 0; i < spawnCount; i++)
    {
        glm::vec3 currPos = Config::getVec3("Cutting_Board_Init_Spawn");
        float variation = Config::getFloat("Cutting_Board_Spacing");
        currPos.x += i * variation;
        Cookware* c = new Cookware(CUTTING_BOARD);
        c->setPosition(currPos);
        mp->cookwareObjects.push_back(c);
    }

    // Add pots (vary by z)
    for (int i = 0; i < spawnCount; i++)
    {
        glm::vec3 currPos = Config::getVec3("Pot_Init_Spawn");
        float variation = Config::getFloat("Pot_Spacing");
        currPos.z += i * variation;
        Cookware* c = new Cookware(POT);
        c->setPosition(currPos);
        mp->cookwareObjects.push_back(c);
    }

    // Add pans (vary by z)
    for (int i = 0; i < spawnCount; i++)
    {
        glm::vec3 currPos = Config::getVec3("Pan_Init_Spawn");
        float variation = Config::getFloat("Pan_Spacing");
        currPos.z += i * variation;
        Cookware* c = new Cookware(PAN);
        c->setPosition(currPos);
        mp->cookwareObjects.push_back(c);
    }

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