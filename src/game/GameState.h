#include <schema/Game.pb.h> 
#include <objects/GameObject.h>
#include <unordered_map>

/**
 * This class holds the game state.
 * This is used by the ServerGame
 * */
class GameState {
private:
    const unsigned int VELOCITY = 1;

    // This is used to increment the counter
    unsigned int objCounter = 0;

    // This is a mapping of clientId to gameObjId
    std::unordered_map<unsigned int, unsigned int> clientIdToGameObjId;

    // This is a mapping of gameObjId to gameObjects
    std::unordered_map<unsigned int, GameObject::GameObject*> gameObjects;

    // This is a mapping of clientId to score
    // std::unordered_map<unsigned int, unsigned int> scoreMap;

public:
    GameState();
    ~GameState();

    /**
     * Adds a new user object using the clientId
     * */
    void addPlayer(unsigned int);

    /**
     * Adds a new default game object class.
     * Note: Use #addPlayer if you are adding a player!
     * */
    int addObject(Game::ObjectType);

    // Gets a player object from the client id
    GameObject* getPlayerObject(unsigned int);

    // Return the reference game object
    GameObject* getGameObject(unsigned int);

    // Remove an object using the object id
    // void removeObject(unsigned int);

    // Removes a player object using the client id
    // void removeUser(unsigned int);
};