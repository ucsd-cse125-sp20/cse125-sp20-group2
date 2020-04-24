#include <game/GameState.h>

GameState::GameState() {

}

GameState::~GameState() {

}

void GameState::addUser(unsigned int clientId) {
    std::cout << "Called add user in GameState w/ clientID: " << clientId << std::endl;

    Game::Vector3 *vec = new Game::Vector3();
    vec->set_x(0);
    vec->set_y(0);
    vec->set_z(0);
    // Inserts into map for specific player
    playerLocations[clientId] = vec; 
}





