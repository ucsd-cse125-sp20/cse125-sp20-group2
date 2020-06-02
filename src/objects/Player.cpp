#include <objects/Player.h>

Player::Player(int ID) : GameObject(ID)
{
    //this->runSpeed = 0;
    //this->turnSpeed = 0;
    this->xSpeed = 0;
    this->zSpeed = 0;
    this->score = 0;
    this->objType = PLAYER;
    this->setModel(Config::get("Player_Model"));
    this->baseRadius = Config::getFloat("Player_Radius");
    this->box->setCircleBoundingBox();
    this->inventory = new std::unordered_map<int, Ingredient*>();
    this->updateMeasurements();
}

void Player::setScore(int newScore)
{
    this->score = newScore;
}

void Player::setClientID(int clientID)
{
    this->clientID = clientID;
}

void Player::setTeamName(std::string teamName)
{
    this->teamName = teamName;
}

void Player::addToScore(int addition)
{
    this->score += addition;
}

int Player::getScore()
{
    return this->score;
}

void Player::setXSpeed( float speed )
{
    this->xSpeed = speed;
}

float Player::getXSpeed()
{
    return this->xSpeed;
}

void Player::setZSpeed ( float speed )
{
    this->zSpeed = speed;
}

float Player::getZSpeed ()
{
    return this->zSpeed;
}

void Player::addToInventory(Ingredient *ingredient)
{
    (*inventory)[ingredient->getID()] = ingredient;
}

void Player::removeFromInventory(Ingredient *ingredient)
{
    inventory->erase(ingredient->getID());
}

int Player::getClientID() { return this->clientID; };

std::string Player::getTeamName() { return this->teamName; }

std::deque<Instruction*> Player::getCompletedInstructions() {
    return this->completedInstructions;
}

void Player::addToCompletedInstructions(Instruction* inst) {
    this->completedInstructions.push_back(inst);
    this->addToScore(inst->points);
}

std::unordered_map<int, Ingredient*>* Player::getInventory() {
    return this->inventory;
}

// pass in uishader->ID as parameter
void Player::drawInventory(GLuint shaderProgram) {
    glDisable(GL_DEPTH_TEST);
    glUseProgram(shaderProgram);

    std::unordered_map<int, Ingredient*>::iterator it = inventory->begin();

    while (it != inventory->end()) {
        it->second->draw();
        it++;
    }
}

void Player::setFreeze(bool f) {
    this->freeze = f;
}

bool Player::getFreeze() {
    return this->freeze;
}

Team Player::getTeam()
{
    return this->team;
}

void Player::setTeam(Team team)
{
    this->team = team;
}