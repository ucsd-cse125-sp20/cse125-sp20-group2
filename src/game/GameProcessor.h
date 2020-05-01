#include <schema/Game.pb.h>
#include <game/GameState.h>

/**
 * This processes information from the server and modifies game state.
 * */
class GameProcessor
{
private:
    GameState state;

public:
    GameProcessor();
    
    ~GameProcessor();

    /**
     * This processes a client message for a client id.
     * This will modify GameState.
     * */
    void process(unsigned int, Game::ClientMessage);
};