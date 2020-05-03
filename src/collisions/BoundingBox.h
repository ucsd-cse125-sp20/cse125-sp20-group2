#pragma once

#include <objects/GameObject.h>
// #include <collisions/RectangleBoundingBox.h>
// #include <collisions/CircleBoundingBox.h>

class GameObject;

class BoundingBox 
{   
public:
    GameObject *obj;
    float radius;
    float minX, minZ, maxX, maxZ;
    int height, width;
    bool isCircle;

    BoundingBox(GameObject* obj);
    ~BoundingBox();

    bool isIntersecting(BoundingBox* bb);

    void updateCorners();

    void setRadius(float radius);

    // virtual bool isIntersecting(CircleBoundingBox*) {
    //     return false;
    // }

    // virtual bool isIntersecting(RectangleBoundingBox*) {
    //     return false;
    // }
};
