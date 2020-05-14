#include <game/GameState.h>

#define PLAYER_RADIUS 0.05

GameState::GameState() {
    /// TODO, implement timer logic
    this->timer = 30;
}

GameState::~GameState() {
    for (auto gameObjectPair : this->gameObjects)
    {
        delete gameObjectPair.second;
    }
    for (auto playerObjectPair : this->playerObjects)
    {
        delete playerObjectPair.second;
    }
}

void GameState::addPlayer(unsigned int clientId) {
    std::cout << "Called add user in GameState w/ clientID: " << clientId << std::endl;

    // First, create player w/ id
    int objId = this->objCounter++;
    Player* newPlayerObject = new Player(objId);
    newPlayerObject->getBoundingBox()->setRadius(0.15);

    // Next, add player to map
    this->playerObjects[clientId] = newPlayerObject;

    // OLD CODE
    // Which id should be used to refer to the player
    // (Can we just use clientId to refer to player object)?
    // int id = addObject(Game::ObjectType::PLAYER);
    // this->clientIdToGameObjId[clientId] = id;

    // return id;

    std::cout << "Returning from add player" << std::endl;
}

void GameState::addMap(Map *map) {
    for(auto it = map->wallList.begin(); it!= map->wallList.end(); it++) {
        this->gameObjects[(*it)->getID()] = *it;
    }

    for(auto it = map->ingredients.begin(); it != map->ingredients.end(); it++) {
        this->gameObjects[(*it)->getID()] = *it;
    }
}

// Adds the object to the object map
int GameState::addObject(Game::ObjectType objectType)
{
    switch (objectType) {
        // Create player object, add to map
        case Game::ObjectType::PLAYER:
        {
            std::cout << "Creating a player object on the server side" << std::endl;
            int objId = this->objCounter++;
            Player* player = new Player(objId);
            player->getBoundingBox()->setRadius(PLAYER_RADIUS);
            this->gameObjects[objId] = player;
            std::cout << "Returning the player object id" <<std::endl;
            return objId;
        }
        // Create fruit object, add to map TODO
        case Game::ObjectType::INGREDIENT:
        {
            std::cout << "Creating a ingredient object on the server side" << std::endl;
            int objId = this->objCounter++;
            IngredientObject* ingredient = new IngredientObject(objId);
            ingredient->getBoundingBox()->setRadius(PLAYER_RADIUS/2);
            this->ingredientObjects[objId] = ingredient;
            std::cout << "Returning the ingredient object id" <<std::endl;
        
        }
        default:
            break;
    }
    return -1;
}

const std::unordered_map<unsigned int, GameObject*>& GameState::getObjects()
{
    // the class contains a unordered map called this->gameObjects
    return this->gameObjects;
}

const std::unordered_map<unsigned int, Player*>& GameState::getPlayerObjects()
{
    return this->playerObjects;
}

const std::unordered_map<unsigned int, IngredientObject*>& GameState::getIngredientObjects() {
    return this->ingredientObjects;
}

// If I called this, and assigned it to the following, what would happen?
//std::unordered_map<unsigned int, GameObject*> myMap = getObjects() (copy?)
//std::unordered_map<unsigned int, GameObject*>& myMap = getObjects() (reference?)

Player* GameState::getPlayerObject(unsigned int clientId)
{
    return this->playerObjects[clientId];

    // OLD CODE
    // int objId = this->clientIdToGameObjId[clientId];
    // return this->gameObjects[objId];
}

GameObject* GameState::getGameObject(unsigned int objId) 
{
    return this->gameObjects[objId];
}

IngredientObject* GameState::getIngredientObject(unsigned int ingredientId)
{
    return this->ingredientObjects[ingredientId];
}

void GameState::removeObject(unsigned int objId)
{
    GameObject* object = this->gameObjects[objId];
    delete object;
    this->gameObjects.erase(objId);
}

void GameState::removePlayer(unsigned int clientId)
{
    // Free player object, remove from player map
    Player* playerObject = this->playerObjects[clientId];
    delete playerObject;
    this->playerObjects.erase(clientId);

    // OLD CODE
    // int objId = this->clientIdToGameObjId[clientId];

    // GameObject* object = this->gameObjects[objId];
    // free(object);

    // this->clientIdToGameObjId.erase(clientId);
    // this->gameObjects.erase(objId);
}

void GameState::removeIngredient(unsigned int ingredientId)
{
    IngredientObject* ingredient = this->ingredientObjects[ingredientId];
    delete ingredient;
    this->ingredientObjects.erase(ingredientId);
}