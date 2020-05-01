#pragma once

#include <objects/GameObject.h>
#include <unordered_map>

class Player : public GameObject 
{
public:
    Player(int ID);

    int score;

    void setScore(int newScore);
};