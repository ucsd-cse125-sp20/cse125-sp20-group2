#pragma once

#include <objects/GameObject.h>
#include <objects/IngredientObject.h>
#include <unordered_map>

class Player : public GameObject 
{
protected:
    int teamID;

    int score  = 0;

    int teamName;

    unordered_map<string, IngredientObject*> inventory;
public:
    Player(int ID);
    
    void setTeamID( int teamID );

    void setTeamName( String teamName );

    void setScore(int newScore)

    void addToScore( int addition );

    void addToInventory( IngredientObject* ingredient );

    void removeFromInventory( IngredientObject* ingredient );
};