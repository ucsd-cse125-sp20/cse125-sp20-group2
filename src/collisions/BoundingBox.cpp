#include <collisions/BoundingBox.h>
#include <iostream>

BoundingBox::BoundingBox(GameObject* gameObject)
{
    this->obj = gameObject;
}

BoundingBox::~BoundingBox()
{
    
}

void BoundingBox::setRadius(float radius ) {
    this->radius = radius;
}

bool BoundingBox::isIntersecting(BoundingBox* bb)
{
    glm::vec3 circlePos1 = obj->getPosition();
    glm::vec3 circlePos2 = bb->obj->getPosition();
    float distance = sqrt((circlePos1.x - circlePos2.x) * (circlePos1.x - circlePos2.x) +
                      (circlePos1.z - circlePos2.z) * (circlePos1.z - circlePos2.z));
    std::cout<<"Distance between spheres "<<distance<<std::endl;
    std::cout<<"Radius 1 "<<radius<<std::endl;
    std::cout<<"Radius 2 "<<bb->radius<<std::endl;
    std::cout<<"Bool" << (distance < (radius + bb->radius)) << std::endl;
    return distance < (radius + bb->radius);
}

void BoundingBox::updateCorners() {
    glm::vec3 position = obj->getPosition();
    maxX = position.x + width/2;
    minX = position.x - width/2;
    maxZ = position.z + height/2;
    minZ = position.z - height/2;
}