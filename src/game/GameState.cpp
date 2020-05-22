#include <game/GameState.h>

#define PLAYER_RADIUS 0.05

GameState::GameState() {
    /// TODO: implement timer logic
    // auto xMinutes = std::chrono::minutes(5);
    auto xMinutes = std::chrono::seconds(15);
    this->round = Game::RoundInfo::LOBBY;
    /// TODO: Round End should start once the Dungeon or Kitchen phase starts
    // this->roundEnd = std::chrono::high_resolution_clock::now() + xMinutes;
    this->oldTime = 0;
}

GameState::~GameState() {
    for (auto objects : this->getAllObjects())
    {
        delete objects;
    }
}

void GameState::addPlayer(unsigned int clientId) {
    std::cout << "Called add user in GameState w/ clientID: " << clientId << std::endl;

    // First, create player w/ id
    int objId = this->objCounter++;
    Player* newPlayerObject = new Player(objId);
    newPlayerObject->setPosition(glm::vec3(0, 0, 0));
    newPlayerObject->setClientID(clientId);

    // Next, add player to map
    this->playerObjects[clientId] = newPlayerObject;
}

void GameState::addMap(Map *map) {

    this->map = map;
    for(auto it = map->wallList.begin(); it!= map->wallList.end(); it++) {
        /// TODO: This needs a new map
        // this->gameObjects[(*it)->getID()] = *it;
    }
}

void GameState::addRecipe(Recipe *recipe) {

    this->recipe = recipe;
    for(auto it = recipe->ingredientList.begin(); it!= recipe->ingredientList.end(); it++) {
        /// TODO: This needs a new map
        // this->gameObjects[(*it)->getID()] = *it;
    }
}

const std::unordered_map<unsigned int, Player*>& GameState::getPlayerObjects()
{
    return this->playerObjects;
}

const std::unordered_map<unsigned int, IngredientObject*>& GameState::getIngredientObjects() {
    return this->ingredientObjects;
}

Player* GameState::getPlayerObject(unsigned int clientId)
{
    return this->playerObjects[clientId];
}

IngredientObject* GameState::getIngredientObject(unsigned int ingredientId)
{
    return this->ingredientObjects[ingredientId];
}

void GameState::removePlayer(unsigned int clientId)
{
    // Free player object, remove from player map
    Player* playerObject = this->playerObjects[clientId];
    delete playerObject;
    this->playerObjects.erase(clientId);
}

void GameState::removeIngredient(unsigned int ingredientId)
{
    IngredientObject* ingredient = this->ingredientObjects[ingredientId];
    delete ingredient;
    this->ingredientObjects.erase(ingredientId);
}

bool GameState::gameOver()
{
    auto currTime = std::chrono::high_resolution_clock::now();
    return currTime > this->roundEnd;
}

int GameState::getRoundTime()
{
    auto currTime = std::chrono::high_resolution_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds> (this->roundEnd - currTime);

    return seconds.count() > 0 ? seconds.count() : 0;
}

bool GameState::timeHasUpdated()
{
    auto roundTime = this->getRoundTime();
    bool result = roundTime != this->oldTime;
    this->oldTime = roundTime;
    return result;
}

void GameState::removeAllObjects()
{
    this->ingredientObjects.clear();
    this->playerObjects.clear();
}

std::vector<GameObject*> GameState::getAllObjects()
{
    std::vector<GameObject*> gameObjectList;

    for (const auto & ingredientPair : this->ingredientObjects)
    {
        gameObjectList.push_back(ingredientPair.second);
    }

    for (const auto & playerPair : this->playerObjects)
    {
        gameObjectList.push_back(playerPair.second);
    }

    return gameObjectList;
}