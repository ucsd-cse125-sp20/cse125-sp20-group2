#include <collisions/BoundingBox.h>
#include <iostream>

// TODO: radius scaling for circle + width/height scaling for rectangle

BoundingBox::BoundingBox(GameObject *gameObject)
{
    this->obj = gameObject;
}

BoundingBox::~BoundingBox()
{
}

bool BoundingBox::isAABBIntersecting(BoundingBox *rbb)
{
    updateCorners();
    rbb->updateCorners();
    return (this->minX <= rbb->maxX && this->maxX >= rbb->minX) &&
           (this->minZ <= rbb->maxZ && this->maxZ >= rbb->minZ);
}

bool BoundingBox::isCircleRectangleIntersecting(BoundingBox *cbb)
{
    updateCorners();
    glm::vec3 circlePos = cbb->obj->getPosition();
    float x = std::max(this->minX, std::min(circlePos.x, this->maxX));
    float z = std::max(this->minZ, std::min(circlePos.z, this->maxZ));

    float distance = std::sqrt((x - circlePos.x) * (x - circlePos.x) +
                               (z - circlePos.z) * (z - circlePos.z));

    return distance < cbb->radius;
}

bool BoundingBox::isCircleIntersecting(BoundingBox *bb)
{
    glm::vec3 circlePos1 = this->obj->getPosition();
    glm::vec3 circlePos2 = bb->obj->getPosition();
    float distance = sqrt((circlePos1.x - circlePos2.x) * (circlePos1.x - circlePos2.x) +
                          (circlePos1.z - circlePos2.z) * (circlePos1.z - circlePos2.z));
    return distance < (this->radius + bb->radius);
}

void BoundingBox::setCircleBoundingBox() { this->isCircle = true; }

void BoundingBox::setWidth(float width)
{
    this->isCircle = false;
    this->width = width;
}

void BoundingBox::setHeight(float height)
{
    this->isCircle = false;
    this->height = height;
}

bool BoundingBox::isCircleBoundingBox()
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
    // TODO: include scaling in calculations
    glm::vec3 position = obj->getPosition();
    maxX = position.x + width / 2;
    minX = position.x - width / 2;
    maxZ = position.z + height / 2;
    minZ = position.z - height / 2;
}