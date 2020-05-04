#include <game/MovementProcessor.h>

MovementProcessor::MovementProcessor() {

}

void MovementProcessor::processMovement(Player *player, Game::Direction dir, int tickCount)
{
    glm::vec3 currentPos = player->getPosition();
    float modelRotationX = player->getRotation();
    float currentTurnSpeed = player->getTurnSpeed();
    float currentRunSpeed = player->getRunSpeed();


    float deltaTime = ((float) tickCount) / (float) 100;
    switch (dir)
    {
    case Game::Direction::UP:
    {
        currentRunSpeed = 1;
        break;
    }
    case Game::Direction::DOWN:
    {
        currentRunSpeed = -1;
        break;
    }
    case Game::Direction::LEFT:
    {
        currentTurnSpeed = 1;
        break;
    }
    case Game::Direction::RIGHT:
    {
        currentTurnSpeed = -1;
        break;
    }
    default: {
        currentRunSpeed = 0;
        currentTurnSpeed = 0;
        break;
    }
    }

    modelRotationX += currentTurnSpeed * deltaTime;
	float distance = currentRunSpeed * deltaTime;
	float dx = distance * sin(modelRotationX);
	float dz = distance * cos(modelRotationX);
	currentPos.x += dx;
	currentPos.z += dz;

    player->setTurnSpeed(0);
    player->setRunSpeed(0);
    player->setPosition(currentPos);
    player->setRotation(modelRotationX);
}