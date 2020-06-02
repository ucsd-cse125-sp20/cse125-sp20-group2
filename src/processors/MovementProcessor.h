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
        float currentXSpeed = player->getXSpeed();
        float currentZSpeed = player->getZSpeed();

        // base speeds, as defined in the config
        float baseRunSpeed = Config::getFloat("Player_Run_Speed");

        float deltaTime = ((float) tickCount) / (float) 100;
        switch (dir)
        {
            case Game::Direction::UP:
            {
                currentZSpeed = -baseRunSpeed;
                player->setRotation(180);
                break;
            }
            case Game::Direction::DOWN:
            {
                currentZSpeed = baseRunSpeed;
                player->setRotation(0);
                break;
            }
            case Game::Direction::LEFT:
            {
                currentXSpeed = -baseRunSpeed;
                player->setRotation(270);
                break;
            }
            case Game::Direction::RIGHT:
            {
                currentXSpeed = baseRunSpeed;
                player->setRotation(90);
                break;
            }
            default: {
                currentXSpeed = 0;
                currentZSpeed = 0;
                break;
            }
        }

        currentPos.x += currentXSpeed * pow(deltaTime, 1.75);
        currentPos.z += currentZSpeed * pow(deltaTime, 1.75);

        player->setXSpeed(0);
        player->setZSpeed(0);
        player->setPosition(currentPos);
    }
};