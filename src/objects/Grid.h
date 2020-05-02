#pragma once
#include <objects/GameObject.h>

class Grid : public GameObject {
public:
    Grid(int ID, int x, int z);
    int x;
    int z;

    void createTiles();
};