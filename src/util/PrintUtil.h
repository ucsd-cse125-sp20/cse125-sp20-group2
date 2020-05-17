#pragma once 

#include <schema/Game.pb.h>
#include <iostream>
#include <spdlog/spdlog.h>

class PrintUtil
{
private:
public:
    static std::shared_ptr<spdlog::logger> _logger;

    static void print(Game::ServerMessage msg);
    static void print(Game::ClientMessage msg);
};
