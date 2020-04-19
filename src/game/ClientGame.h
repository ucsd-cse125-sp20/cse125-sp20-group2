#include <network/NetworkClient.h>
#include <iostream>
#include <util/PrintUtil.h>

class ClientGame {
    public:
        ClientGame(std::string IP, int port);
        ~ClientGame();
        void runGame();
    private:
        NetworkClient client;
        void sendMsgs();
        void receiveUpdates();
        void updateGameState();
        void renderWorld();
};