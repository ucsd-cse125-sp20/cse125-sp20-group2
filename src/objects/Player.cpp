#include <objects/Player.h>

Player::Player(int ID) : GameObject(ID) {

}

void Player::setScore(int newScore) {
    score = newScore;
}