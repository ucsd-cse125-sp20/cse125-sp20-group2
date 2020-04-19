#include <game/ServerGame.h>
#include <util/PrintUtil.h>
#include <thread>
#include <chrono>

#define TICK 30

ServerGame::ServerGame(int port) : server(port)
{
    run();
}

ServerGame::~ServerGame()
{

}

void ServerGame::run()
{
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        auto map = server.readAllMessages();
        
        // TODO: Process 
        
        // Go through all clients
        for (auto iter = map.begin(); iter != map.end(); iter++) {
            auto msgs = iter->second;
            for (auto msg: msgs) {
                PrintUtil::print(msg);
            }
        }

        Game::ServerMessage msg;
        msg.set_net(Game::PING);
        server.sendToAll(msg);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dur = end - start;

        std::chrono::duration<double> sleepTime = std::chrono::milliseconds(TICK) - dur;

        if (sleepTime.count() >= 0) {
            std::cout << "Time to process: " << dur.count() << " Sleeping for: " << sleepTime.count() << std::endl;
            std::this_thread::sleep_for(sleepTime);
        } else {
            std::cout << "Server is behind by: " << dur.count() << " no panic " << std::endl;
        }
    }
}