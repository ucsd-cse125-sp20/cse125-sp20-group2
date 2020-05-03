#include <objects/Player.h>

Player::Player(int ID) : GameObject(ID)
{
}

void Player::setScore(int newScore)
{
    score = newScore;
}

void Player::setTeamID(int teamID)
{
    this->teamID = teamID;
}

void Player::setTeamName(String teamName)
{
    this->teamName = teamName;
}
void Player::addToScore(int addition)
{
    this->score += addToScore
}

void Player::addToInventory(IngredientObject *ingredient)
{
    inventory[ingredient->getID()] = ingredient;
}

void Player::removeFromInventory(IngredientObject *ingredient)
{
    inventory[ingredient->getID()] = null;
}