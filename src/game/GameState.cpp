#include <game/GameState.h>
#include <objects/Player.h>

GameState::GameState() {

}

GameState::~GameState() {

}

void GameState::addPlayer(unsigned int clientId) {
    std::cout << "Called add user in GameState w/ clientID: " << clientId << std::endl;

    int id = addObject(Game::ObjectType::PLAYER);
    this->clientIdToGameObjId[clientId] = id;
}

// Adds the object to the object map
int GameState::addObject(Game::ObjectType objectType)
{
    switch (objectType) {
        // Create player object, add to map
        case Game::ObjectType::PLAYER:
        {
            Player* newPlayer = new Player("assets\\models\\Basic_Character_Model.obj", glm::vec3(0, -1, 0), 0.2);
            int objId = this->objCounter++;
            this->gameObjects[objId] = newPlayer;
            return objId;
        }
        // Create fruit object, add to map TODO
        case Game::ObjectType::FRUITY:
        {

        }
        default:
            break;
    }
}





