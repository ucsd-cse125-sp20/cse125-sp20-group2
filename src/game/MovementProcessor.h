#pragma once

#include <game/GameProcessor.h>
#include <objects/Player.h>
#include <utility>

class MovementProcessor {
public:
    MovementProcessor();
    void processMovement(Player * player, Game::Direction dir, int tickCount);
};