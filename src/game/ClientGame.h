#include <network/NetworkClient.h>
#include <iostream>
#include <util/PrintUtil.h>
#include <graphics/window.h>
#include <objects/Player.h>

class ClientGame {
    public:
        ClientGame(std::string IP, int port);
        ~ClientGame();
        void runGame();
        int clientID;
        //Player player;      // TODO: decide permanent location of player object
    private:
        Window window;
        NetworkClient client;
        void sendMsgs();
        void receiveUpdates();
        void updateGameState();
        void processInput();
};