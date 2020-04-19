#include <schema/Game.pb.h>
#include <iostream>

class PrintUtil
{
private:
    /* data */
public:
    static void print(Game::ServerMessage msg) {
        // std::cout << "ServerMessage: " << msg.GetDescriptor()->name() << std::endl;
   switch (msg.event_case()) {
       case Game::ServerMessage::EventCase::kNet:
           std::cout << "Net " << msg.net() << std::endl;
           break;
       case Game::ServerMessage::EVENT_NOT_SET:
           std::cout << "undefined server message" << std::endl;
       break;
   }
    }
    
    static void print(Game::ClientMessage msg) {
        // std::cout << "ClientMessage: " << msg.GetDescriptor()->name() << std::endl;
       switch (msg.event_case()) {
           case Game::ClientMessage::EventCase::kDirection:
               std::cout << "Direction: " << msg.direction() << std::endl;
           break;
           case Game::ClientMessage::EventCase::kNet:
               std::cout << "Net: " << msg.net() << std::endl;
           break;
           case Game::ClientMessage::EVENT_NOT_SET:
               std::cout << "undefined client message" << std::endl;
           break;
       }
    }
};

