#pragma once

#include <objects/GameObject.h>
#include <unordered_map>

class Player : public GameObject 
{
public:
    Player(int ID);

    std::unordered_map<int, GameObject> inventory;
    int score;

    // Will probably be used on item pickup in dungeon phase
    void addItem(int index, GameObject item);

    // Will probably be used on item placement in cooking phase
    void removeItem(int index);

    // Will probably be used on item prep in cooking phase
    void replaceItem(int index, GameObject item);

    void setScore(int newScore);
};