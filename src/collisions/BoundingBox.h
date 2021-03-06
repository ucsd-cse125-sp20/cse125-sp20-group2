#pragma once

#include <objects/GameObject.h>
// #include <collisions/RectangleBoundingBox.h>
// #include <collisions/CircleBoundingBox.h>

class GameObject;

class BoundingBox
{
protected:
    GameObject *obj;
    float radius;
    float minX, minZ, maxX, maxZ;
    float height, width, depth;
    bool isCircle;

    bool isAABBIntersecting(BoundingBox *rbb);

    bool isCircleRectangleIntersecting(BoundingBox *cbb);

    bool isCircleIntersecting(BoundingBox *bb);

public:
    BoundingBox(GameObject *obj);
    ~BoundingBox();

    void setRadius(float radius);

    float getRadius();

    bool isIntersecting(BoundingBox *bb);

    void updateCorners();

    bool isCircleBoundingBox();

    void setCircleBoundingBox();

    void setWidth(float width);

    void setDepth(float depth);

    void setHeight(float height);

    // virtual bool isIntersecting(CircleBoundingBox*) {
    //    return false;
    // }

    // virtual bool isIntersecting(RectangleBoundingBox*) {
    //     return false;
    // }
};
