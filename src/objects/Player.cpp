#include <objects/Player.h>

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