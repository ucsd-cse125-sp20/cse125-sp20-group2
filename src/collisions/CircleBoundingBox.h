#pragma once

#include <collisions/BoundingBox.h>
#include <collisions/RectangleBoundingBox.h>

class RectangleBoundingBox;

class CircleBoundingBox : public BoundingBox {
public:
    float radius;

    CircleBoundingBox(GameObject* obj);

    CircleBoundingBox(GameObject* obj, float radius);

    virtual bool isIntersecting(BoundingBox* bb);

    // bool isIntersecting(RectangleBoundingBox *rbb);

    // bool isIntersecting(CircleBoundingBox *cbb);
};