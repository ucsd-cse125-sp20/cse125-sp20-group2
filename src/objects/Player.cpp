#include <objects/Player.h>

// Constructors
Player::Player(std::string modelPath, glm::vec3 worldPos, float uniformScale, int ID) :
GameObject(modelPath, worldPos, uniformScale), clientID(ID) {}
Player::Player(std::string modelPath, glm::vec3 worldPos, glm::vec3 scaleVec, int ID) :
GameObject(modelPath, worldPos, scaleVec), clientID(ID) {}
Player::Player(std::string modelPath, float x, float y, float z, float uniformScale, int ID) :
GameObject(modelPath, x, y, z, uniformScale), clientID(ID) {}
Player::Player(std::string modelPath, float x, float y, float z, float sX, float sY, float sZ, int ID) :
GameObject(modelPath, x, y, z, sX, sY, sZ), clientID(ID) {}

// Will probably be used on item pickup in dungeon phase
void Player::addItem(int index, GameObject item) {
    inventory.insert({index, item});
}

// Will probably be used on item placement in cooking phase
void Player::removeItem(int index) {
    inventory.erase(index);
}

// Will probably be used on item prep in cooking phase
void Player::replaceItem(int index, GameObject item) {
    inventory.erase(index);
    inventory.insert({index, item});
}

void Player::setScore(int newScore) {
    score = newScore;
}