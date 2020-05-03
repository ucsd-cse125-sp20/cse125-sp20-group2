#include <game/GameState.h>
#include <objects/Player.h>

GameState::GameState() {

}

GameState::~GameState() {

}

int GameState::addPlayer(unsigned int clientId) {
    std::cout << "Called add user in GameState w/ clientID: " << clientId << std::endl;

    int id = addObject(Game::ObjectType::PLAYER);
    this->clientIdToGameObjId[clientId] = id;

    return id;
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
            player->getBoundingBox()->setRadius( 0.25 );
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

// If I called this, and assigned it to the following, what would happen?
//std::unordered_map<unsigned int, GameObject*> myMap = getObjects() (copy?)
//std::unordered_map<unsigned int, GameObject*>& myMap = getObjects() (reference?)

GameObject* GameState::getPlayerObject(unsigned int clientId)
{
    int objId = this->clientIdToGameObjId[clientId];
    return this->gameObjects[objId];
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
    int objId = this->clientIdToGameObjId[clientId];

    GameObject* object = this->gameObjects[objId];
    free(object);

    this->clientIdToGameObjId.erase(clientId);
    this->gameObjects.erase(objId);
}