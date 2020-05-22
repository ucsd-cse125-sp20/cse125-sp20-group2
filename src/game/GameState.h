#pragma once

#include <schema/Game.pb.h> 
#include <objects/GameObject.h>
#include <unordered_map>
#include <objects/Player.h>
#include <objects/IngredientObject.h>
#include <objects/Map.h>
#include <util/Recipe.h>
#include <utility>
#include <util/Config.h>
#include <chrono>

/**
 * This class holds the game state.
 * This is used by the ServerGame
 * */
class GameState {
private:
    const unsigned int VELOCITY = 1;

    // This is used to increment the counter
    unsigned int objCounter = 0;

    // This is a mapping of clientId to Player objects
    /// TODO: Map from team id to allow instructor to quickly access their team's player
    std::unordered_map<unsigned int, Player*> playerObjects;

    // This is a mapping of gameObjId to gameObjects
    std::unordered_map<unsigned int, GameObject*> gameObjects;

    // This is a mapping of gameObjId to ingredient objects
    std::unordered_map<unsigned int, IngredientObject*> ingredientObjects;

    /// TODO: Implement
    // This is a mapping of clientId to score
    // std::unordered_map<unsigned int, unsigned int> scoreMap;
    Recipe* recipe; 

    Map* map;

    // A number representing the current round
    int round;

    // The time when the current round ends
    std::chrono::high_resolution_clock::time_point roundEnd;

    // Keeps track of previous time
    int64_t oldTime;

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

    /**
     * Gets a player object from the client id
     * */
    Player* getPlayerObject(unsigned int);

    /**
     * Return the reference game object
     * */
    GameObject* getGameObject(unsigned int);

    /**
     * Return the reference ingredient object
     * */
    IngredientObject* getIngredientObject(unsigned int);

    /**
     * Adds objects in map
     * */
    void addMap(Map *map);

     /**
     * Add ingredient objects in map
     * */
    void addRecipe(Recipe *recipe);

    /**
     * Returns all game objects
     * TODO: This currently returns a copy
     * */
    const std::unordered_map<unsigned int, GameObject*>& getObjects();

    /**
     * Returns all player objects
     * TODO: This currently returns a copy
     * */
    const std::unordered_map<unsigned int, Player*>& getPlayerObjects();

    /**
     * Returns all ingredient objects
     * TODO: This currently returns a copy
     * */
    const std::unordered_map<unsigned int, IngredientObject*>& getIngredientObjects();

    /**
     * Removes a player object using the client id
     * */
    void removePlayer(unsigned int);

    /**
     * Remove an object using the object id
     * */
    void removeObject(unsigned int);

    /**
     * Remove a ingredient object using the object id
     * */
    void removeIngredient(unsigned int);

    /**
     * Used to determine if the current round is over
     * */
    bool gameOver();

    /**
     * Used to retrieve the amount of time before the round ends.
     * This returns the time unit in seconds.
     * */
    int getRoundTime();

    /**
     * Used to determine if the timer value has updated, and should
     * be sent over to the clients
     * */
    bool timeHasUpdated();
};