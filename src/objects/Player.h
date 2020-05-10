#pragma once

#include <objects/GameObject.h>
#include <objects/IngredientObject.h>
#include <unordered_map>
#include <string>

class Player : public GameObject 
{
protected:
    int teamID;

    int score  = 0;

    std::string teamName;

    float runSpeed = 0, turnSpeed = 0;

    std::unordered_map<int, IngredientObject*> inventory;

    std::deque<Instruction*> completedInstructions;
public:
    Player(int ID);

    std::deque<Instruction*> getCompletedInstructions();

    void addToCompletedInstructions(Instruction* inst);
    
    void setTeamID( int teamID );

    void setTeamName( std::string teamName );

    int getTeamID();

    void setRunSpeed( float newRunSpeed );

    void setTurnSpeed( float newTurnSpeed );

    float getRunSpeed();

    float getTurnSpeed();

    std::string getTeamName();

    void setScore(int newScore);

    void addToScore( int addition );

    void addToInventory( IngredientObject* ingredient );

    void removeFromInventory( IngredientObject* ingredient );
};