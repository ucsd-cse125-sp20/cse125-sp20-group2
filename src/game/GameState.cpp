#include <game/GameState.h>

#define PLAYER_RADIUS 0.05

GameState::GameState() {
    this->round = Game::RoundInfo::LOBBY;
}

GameState::~GameState() {
    for (auto objects : this->getAllObjects())
    {
        delete objects;
    }
}

Player* GameState::addPlayer(unsigned int clientId) {
    std::cout << "Called add user in GameState w/ clientID: " << clientId << std::endl;

    // First, create player w/ id
    int objId = this->objCounter++;
    Player* newPlayerObject = new Player(objId);
    newPlayerObject->setPosition(glm::vec3(0, 0, 0));
    newPlayerObject->setClientID(clientId);

    // Next, add player to map
    this->playerObjects[clientId] = newPlayerObject;
    return newPlayerObject;
}

void GameState::addMap(Map *map) {

    this->map = map;
    for(auto it = map->wallList.begin(); it!= map->wallList.end(); it++) {
        this->worldObjects[(*it)->getID()] = *it;
    }
}

void GameState::addRecipe(Recipe *recipe) {

    this->recipe = recipe;
    for(auto it = recipe->ingredientList.begin(); it!= recipe->ingredientList.end(); it++) {
        this->worldObjects[(*it)->getID()] = *it;
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
    switch (this->round)
    {
        // Game over if all players are ready
        case Game::RoundInfo_RoundState_LOBBY:
        {
            return this->allClientsReady();
        }
        // Game over if 15 seconds over (TODO, change logic)
        /// TODO: Modify logic
        case Game::RoundInfo_RoundState_DUNGEON_WAITING:
        { 
            break;
        }
        // Game over if some time is over
        case Game::RoundInfo_RoundState_DUNGEON:
        {
            break;
        }
        // Game over if 15 seconds is over
        case Game::RoundInfo_RoundState_KITCHEN_WAITING:
        {
            break;
        }
        // Game over if some time is over
        case Game::RoundInfo_RoundState_KITCHEN:
        {
            break;
        }
        // Never in game over
        case Game::RoundInfo_RoundState_END:
        {
            break;
        }
        
        default:
            break;
    }

    auto currTime = std::chrono::high_resolution_clock::now();
    return currTime > this->roundEnd;
}

bool GameState::allClientsReady()
{
    // If no clients connected, return false
    if (this->readyStatus.size() == 0)
    {
        return false;
    }

    for (auto clientPair : this->readyStatus)
    {
        bool clientIsReady = clientPair.second;
        if (!clientIsReady)
        {
            return false;
        }
    }
    return true;
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

    for (const auto & worldPair : this->worldObjects)
    {
        gameObjectList.push_back(worldPair.second);
    }

    return gameObjectList;
}

Game::RoundInfo::RoundState GameState::getRound()
{
    return this->round;
}


void GameState::setRound(Game::RoundInfo::RoundState round)
{
    this->round = round;
}

void GameState::advanceRound()
{
    switch (this->round)
    {
        case Game::RoundInfo::LOBBY :
        {
            this->round = Game::RoundInfo::DUNGEON_WAITING;
            break;
        }
        case Game::RoundInfo::DUNGEON_WAITING :
        {
            this->round = Game::RoundInfo::DUNGEON;
            break;
        }
        case Game::RoundInfo::DUNGEON :
        {
            this->round = Game::RoundInfo::KITCHEN_WAITING;
            break;
        }
        case Game::RoundInfo::KITCHEN_WAITING :
        {
            this->round = Game::RoundInfo::KITCHEN;
            break;
        }
        case Game::RoundInfo::KITCHEN :
        {
            this->round = Game::RoundInfo::END;
            break;
        }
        case Game::RoundInfo::END :
        {
            this->round = Game::RoundInfo::LOBBY;
            break;
        }
        default:
            break;
    }
}

void GameState::setRoundTime(unsigned int seconds)
{
    auto xMinutes = std::chrono::seconds(seconds);
    this->roundEnd = std::chrono::high_resolution_clock::now() + xMinutes;
    this->oldTime = 0;
}