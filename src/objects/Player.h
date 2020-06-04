#pragma once

#include <objects/GameObject.h>
#include <objects/Ingredient.h>
#include <util/Recipe.h>
#include <unordered_map>
#include <string>
#include <util/Instruction.h>
#include <deque>

enum Team
{
    BLUE = 0,
    GREEN = 1,
    PURPLE = 2,
    RED = 3
};

class Player : public GameObject 
{
protected:
    int clientID;

    int score  = 0;

    bool freeze;

    std::string teamName;

    float runSpeed = 0, turnSpeed = 0;

    std::unordered_map<int, Ingredient*>* inventory;

    std::deque<Instruction*> completedInstructions;

    void loadCollisionSize();

    Team team;

public:
    ///TODO: this is extremely janky. try to find a different way to determine movement
    int noMoveCounter;
    bool moving;
    bool cooking;

    Player(int ID);

    void setFreeze(bool f);

    bool getFreeze();

    std::vector<std::pair<Instruction*, bool>> instructionSet;

    std::deque<Instruction*> getCompletedInstructions();

    void addToCompletedInstructions(Instruction* inst);
    
    void setClientID( int clientID );

    void setTeamName( std::string teamName );

    int getClientID();

    void setRunSpeed( float newRunSpeed );

    void setTurnSpeed( float newTurnSpeed );

    float getRunSpeed();

    float getTurnSpeed();

    std::string getTeamName();

    void setScore(int newScore);

    void addToScore( int addition );

    int getScore();

    void addToInventory( Ingredient* ingredient );

    void removeFromInventory( Ingredient* ingredient );

    std::unordered_map<int, Ingredient*>* getInventory();

    void drawInventory(GLuint shaderProgram);

    Team getTeam();

    void setTeam(Team team);
};