#include <game/GameState.h>
#include <objects/Player.h>

GameState::GameState() {

}

GameState::~GameState() {

}

int GameState::addPlayer(unsigned int clientId) {
    std::cout << "Called add user in GameState w/ clientID: " << clientId << std::endl;

    // First, create player w/ id
    int objId = this->objCounter++;
    Player* newPlayerObject = new Player(objId);
    newPlayerObject->getBoundingBox()->setRadius(0.15);

    // Next, add player to map
    this->playerObjects[clientId] = newPlayerObject;

    // Placeholder, just returns client id
    return clientId;

    // OLD CODE
    // Which id should be used to refer to the player
    // (Can we just use clientId to refer to player object)?
    // int id = addObject(Game::ObjectType::PLAYER);
    // this->clientIdToGameObjId[clientId] = id;

    // return id;

    std::cout << "Returning from add player" << std::endl;
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
            player->getBoundingBox()->setRadius( 0.05 );
            this->gameObjects[objId] = player;
            std::cout << "Returning the player object id" <<std::endl;
            return objId;
        }
        // Create fruit object, add to map TODO
        case Game::ObjectType::FRUITY:
        {

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

void GameState::removeObject(unsigned int objId)
{
    GameObject* object = this->gameObjects[objId];
    free(object);
    this->gameObjects.erase(objId);
}

void GameState::removePlayer(unsigned int clientId)
{
    // Free player object, remove from player map
    Player* playerObject = this->playerObjects[clientId];
    free(playerObject);
    this->playerObjects.erase(clientId);

    // OLD CODE
    // int objId = this->clientIdToGameObjId[clientId];

    // GameObject* object = this->gameObjects[objId];
    // free(object);

    // this->clientIdToGameObjId.erase(clientId);
    // this->gameObjects.erase(objId);
}