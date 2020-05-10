#include <objects/Player.h>

Player::Player(int ID) : GameObject(ID)
{
    this->runSpeed = 0;
    this->turnSpeed = 0;
    this->score = 0;
    this->objType = PLAYER;
}

void Player::setScore(int newScore)
{
    score = newScore;
}

void Player::setTeamID(int teamID)
{
    this->teamID = teamID;
}

void Player::setTeamName(std::string teamName)
{
    this->teamName = teamName;
}
void Player::addToScore(int addition)
{
    this->score += addition;
}

void Player::setRunSpeed(float newRunSpeed) { this->runSpeed = newRunSpeed; }

void Player::setTurnSpeed(float newTurnSpeed) { this->turnSpeed = newTurnSpeed; }

float Player::getRunSpeed() { return this->runSpeed; }

float Player:: getTurnSpeed() { return this->turnSpeed; }

void Player::addToInventory(IngredientObject *ingredient)
{
    inventory[ingredient->getID()] = ingredient;
}

void Player::removeFromInventory(IngredientObject *ingredient)
{
    inventory.erase(ingredient->getID());
}

int Player::getTeamID() { return this->teamID; };

std::string Player::getTeamName() { return this->teamName; }

std::deque<Instruction*> Player::getCompletedInstructions() {
    return this->completedInstructions;
}

void Player::addToCompletedInstructions(Instruction* inst) {
    this->completedInstructions.push_back(inst);
    this->addToScore(inst->score);
}