#pragma once

#include <objects/GameObject.cpp>
#include <unordered_map>

class Player : public GameObject 
{
public:
    // Constructors
    Player(std::string modelPath, glm::vec3 worldPos, float uniformScale);
	Player(std::string modelPath, glm::vec3 worldPos, glm::vec3 scaleVec);
	Player(std::string modelPath, float x, float y, float z, float uniformScale);
	Player(std::string modelPath, float x, float y, float z, float sX, float sY, float sZ);

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