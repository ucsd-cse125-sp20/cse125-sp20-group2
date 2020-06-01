#pragma once

#include <processors/GameProcessor.h>
#include <objects/Player.h>
#include <utility>
#include <util/Config.h>
#include <cmath>

class MovementProcessor {
public:
    static void processMovement(Player * player, Game::Direction dir, int tickCount)
    {
        glm::vec3 currentPos = player->getPosition();
        float modelRotationX = player->getRotation();
        float currentTurnSpeed = player->getTurnSpeed();
        float currentRunSpeed = player->getRunSpeed();

        // base speeds, as defined in the config
        float baseRunSpeed = Config::getFloat("Player_Run_Speed");
        float baseTurnSpeed = Config::getFloat("Player_Turn_Speed");

        float deltaTime = ((float) tickCount) / (float) 100;
        switch (dir)
        {
            case Game::Direction::UP:
            {
                currentRunSpeed = baseRunSpeed;
                break;
            }
            case Game::Direction::DOWN:
            {
                currentRunSpeed = -baseRunSpeed;
                break;
            }
            case Game::Direction::LEFT:
            {
                currentTurnSpeed = baseTurnSpeed;
                break;
            }
            case Game::Direction::RIGHT:
            {
                currentTurnSpeed = -baseTurnSpeed;
                break;
            }
            default: {
                currentRunSpeed = 0;
                currentTurnSpeed = 0;
                break;
            }
        }

        modelRotationX += currentTurnSpeed * pow(deltaTime, 2);
        float distance = currentRunSpeed * pow(deltaTime, 1.75);
        float dx = distance * sin(modelRotationX);
        float dz = distance * cos(modelRotationX);
        currentPos.x += dx;
        currentPos.z += dz;

        player->setTurnSpeed(0);
        player->setRunSpeed(0);
        player->setPosition(currentPos);
        player->setRotation(modelRotationX);
    }
};