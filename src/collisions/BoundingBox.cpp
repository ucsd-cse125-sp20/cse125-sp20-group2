#include <collisions/BoundingBox.h>
#include <iostream>

BoundingBox::BoundingBox(GameObject *gameObject)
{
    this->obj = gameObject;
}

BoundingBox::~BoundingBox()
{
}

bool isAABBIntersecting(BoundingBox *rbb)
{
    updateCorners();
    bb->updateCorners();
    return (this->minX <= rbb->maxX && this->maxX >= rbb->minX) &&
           (this->minZ <= rbb->maxZ && this->maxZ >= rbb->minZ);
}

bool isCircleRectangleIntersecting(BoundingBox *cbb)
{
    updateCorners();
    glm::vec3 circlePos = cbb->obj->getPosition();
    float x = std::max(this->minX, std::min(circlePos.x, this->maxX));
    float z = std::max(this->minZ, std::min(circlePos.z, this->maxZ));

    float distance = std::sqrt((x - circlePos.x) * (x - circlePos.x) +
                               (z - circlePos.z) * (z - circlePos.z));

    return distance < cbb->radius;
}

bool isCircleIntersecting(BoundingBox *bb)
{
    glm::vec3 circlePos1 = obj->getPosition();
    glm::vec3 circlePos2 = bb->obj->getPosition();
    float distance = sqrt((circlePos1.x - circlePos2.x) * (circlePos1.x - circlePos2.x) +
                          (circlePos1.z - circlePos2.z) * (circlePos1.z - circlePos2.z));
    return distance < (radius + bb->radius);
}

void BoundingBox::setCircleBoundingBox() { this->isCircle = true; }

void BoundingBox::setWidth(float width)
{
    this->isCircle = false;
    this->width = width;
}

void BoundingBox::setHeight(float height)
{
    this->isCirlce = false;
    this->height = height;
}

void BoundingBox::isCircle()
{
    return this->isCircle;
}

void BoundingBox::setRadius(float radius)
{
    setCircleBoundingBox();
    this->radius = radius;
}

bool BoundingBox::isIntersecting(BoundingBox *bb)
{
    if (this->isCircle && bb->isCircle)
    {
        return this->isCircleIntersecting(bb);
    }
    else if (this->isCircle)
    {
        return bb->isCircleRectangleIntersecting(this);
    }
    else if (bb->isCircle)
    {
        return this->isCircleRectangleIntersecting(bb);
    }
    else
    {
        return this->isAABBIntersecting(bb);
    }
}

void BoundingBox::updateCorners()
{
    glm::vec3 position = obj->getPosition();
    maxX = position.x + width / 2;
    minX = position.x - width / 2;
    maxZ = position.z + height / 2;
    minZ = position.z - height / 2;
}