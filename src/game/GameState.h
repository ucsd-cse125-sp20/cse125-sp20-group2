#include <unordered_map>
#include <schema/Game.pb.h> 

class GameState {
private:
    const unsigned int VELOCITY = 1;

public:
    GameState();
    ~GameState();

    void addUser(unsigned int);
    
    // TODO: Abstract this into a game object -> player class where the player has vector data, inventory, etc.
    std::unordered_map<unsigned int, Game::Vector3*> playerLocations;

    // Might need player speed

    /**
     * [
     * [(0,0) x x]
     * [x x x]
     * [x x (2,2)]
     * ]
     * */

    // "w w w..."?
    // How I visualize it  w w w w w w w w w w --> server  vs (w, 0.5 second) (w, 1 second) --> server
    std::unordered_map<unsigned int, unsigned int> playerVelocity;

    std::unordered_map<unsigned int, unsigned int> scoreMap;
};