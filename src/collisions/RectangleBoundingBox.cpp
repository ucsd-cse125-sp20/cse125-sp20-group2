// #include <collisions/RectangleBoundingBox.h>

// RectangleBoundingBox::RectangleBoundingBox(GameObject* obj, float width, float height) : BoundingBox(obj) {
//     this->width = width;
//     this->height = height;
//     updateCorners();
// }

// // For now, we are assuming the rectangle has not been rotated
// void RectangleBoundingBox::updateCorners() {
//     glm::vec3 position = obj->getPosition();
//     maxX = position.x + width/2;
//     minX = position.x - width/2;
//     maxZ = position.z + height/2;
//     minZ = position.z - height/2;
//     // corner1.x = obj->getPosition().x + width/2 * cos(obj->getRotation()) + height/2 * sin(obj->getRotation());
//     // corner1.y = obj->getPosition().x + width/2 * cos(obj->getRotation()) + height/2 * sin(obj->getRotation());

//     // corner2.x = obj->getPosition().x - width/2 * cos(obj->getRotation()) + height/2 * sin(obj->getRotation());
//     // corner2.y = obj->getPosition().x - width/2 * cos(obj->getRotation()) + height/2 * sin(obj->getRotation());

//     // corner3.x = obj->getPosition().x + width/2 * cos(obj->getRotation()) - height/2 * sin(obj->getRotation());
//     // corner3.y = obj->getPosition().x + width/2 * cos(obj->getRotation()) - height/2 * sin(obj->getRotation());

//     // corner4.x = obj->getPosition().x - width/2 * cos(obj->getRotation()) - height/2 * sin(obj->getRotation());
//     // corner4.y = obj->getPosition().x - width/2 * cos(obj->getRotation()) - height/2 * sin(obj->getRotation());

//     // // Find minX, maxX, minZ, maxZ from 4 corners
//     // maxX = (corner1.x >= corner2.x && corner1.x >= corner3.x && corner1.x >= corner4.x) ? corner1 : 
//     //     (corner2.x >= corner3.x && corner2.x >= corner4.x) ? corner2 : 
//     //     (corner3.x >= corner4.x) ? corner3 : corner4;
//     // minX = (corner1.x <= corner2.x && corner1.x <= corner3.x && corner1.x <= corner4.x) ? corner1 : 
//     //     (corner2.x <= corner3.x && corner2.x <= corner4.x) ? corner2 : 
//     //     (corner3.x <= corner4.x) ? corner3 : corner4;
//     // maxZ = (corner1.y >= corner2.y && corner1.y >= corner3.y && corner1.y >= corner4.y) ? corner1 : 
//     //     (corner2.y >= corner3.y && corner2.y >= corner4.y) ? corner2 : 
//     //     (corner3.y >= corner4.y) ? corner3 : corner4;
//     // minZ = (corner1.y <= corner2.y && corner1.y <= corner3.y && corner1.y <= corner4.y) ? corner1 : 
//     //     (corner2.y <= corner3.x && corner2.y <= corner4.y) ? corner2 : 
//     //     (corner3.y <= corner4.y) ? corner3 : corner4;
        

// }



// bool RectangleBoundingBox::isIntersecting(RectangleBoundingBox *rbb) {
//    updateCorners();
//    rbb->updateCorners();
//    return (this -> minX <= rbb->maxX && this -> maxX >= rbb->minX) &&
//     (this -> minZ <= rbb -> maxZ && this -> maxZ >= rbb -> minZ);
// }

// bool RectangleBoundingBox::isIntersecting(CircleBoundingBox *cbb) {
//    updateCorners();
//    glm::vec3 circlePos = cbb->obj->getPosition();
//    float x = std::max(this -> minX, std::min(circlePos.x, this -> maxX));
//    float z = std::max(this -> minZ, std::min(circlePos.z, this -> maxZ));

//    float distance = std::sqrt((x - circlePos.x) * (x - circlePos.x) +
//                       (z - circlePos.z) * (z - circlePos.z));

//    return distance < cbb->radius;
// }

