#include <schema/Game.pb.h>
#include <iostream>

class PrintUtil
{
private:
    /* data */
public:
    static void print(Game::ServerMessage msg) {
        std::cout << msg.DebugString() << std::endl;
    }
    
    static void print(Game::ClientMessage msg) {
        std::cout << msg.DebugString() << std::endl;
    }
};

