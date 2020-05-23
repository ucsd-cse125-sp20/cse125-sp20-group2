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
 * USE: ServerGame
 * */
class GameState {
private:
    const unsigned int VELOCITY = 1;

    // This is used to increment the counter
    unsigned int objCounter = 0;

    std::unordered_map<unsigned int, GameObject*> worldObjects;

    // This is a mapping of client id to Player objects
    /// TODO: Map from team id to allow instructor to quickly access their team's player
    std::unordered_map<unsigned int, Player*> playerObjects;

    // This is a mapping of gameObjId to ingredient objects
    std::unordered_map<unsigned int, IngredientObject*> ingredientObjects;

    Recipe* recipe; 

    Map* map;

    // An enum representing the current round
    // Lobby - Represents phase where players are waiting
    // DungeonWaiting - This can be used to show a tutorial and memes
    //                  This should load any requirements for Dungeon
    // Dungeon - Represents phase of collecting ingredients
    // KitchenWaiting - This can be used to show a tutorial and memes
    //                  This should load any requirements for Kitchen
    // Kitchen - Represents the phase to combine ingredients
    Game::RoundInfo::RoundState round;

    // The time when the current round ends
    std::chrono::high_resolution_clock::time_point roundEnd;

    // Keeps track of previous time since last tick
    // Used to tell when a second has passed in GameState#timeHasUpdated
    int64_t oldTime;

public:
    GameState();
    ~GameState();

    /**
     * Adds a new user object using the clientId
     * */
    Player* addPlayer(unsigned int);

    /**
     * Gets a player object from the client id
     * */
    Player* getPlayerObject(unsigned int);

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
     * Returns all player objects
     * */
    const std::unordered_map<unsigned int, Player*>& getPlayerObjects();

    /**
     * Returns all ingredient objects
     * */
    const std::unordered_map<unsigned int, IngredientObject*>& getIngredientObjects();

    /**
     * Removes a player object using the client id
     * */
    void removePlayer(unsigned int);

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

    /**
     * Used to remove all objects
     * */
    void removeAllObjects();
    
    /**
     *  Used to get all objects
     * */
    std::vector<GameObject*> getAllObjects();

    /**
     * Used to retrieve the current round
     * */
    Game::RoundInfo::RoundState getRound();

    /**
     * Used to set the game state's round
     * */
    void setRound(Game::RoundInfo::RoundState);

    /**
     * Used to advance to the next round (for the gamestate)
     * */
    void advanceRound();
};