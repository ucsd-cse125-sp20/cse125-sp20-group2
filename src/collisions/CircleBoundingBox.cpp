// #include <collisions/CircleBoundingBox.h>

// //class CircleBoundingBox : public BoundingBox {
// //public:
// //    float radius;
// //
// //};

// CircleBoundingBox::CircleBoundingBox(GameObject* obj) : BoundingBox(obj) {
// }

// CircleBoundingBox::CircleBoundingBox(GameObject* obj, float radius) : BoundingBox(obj) {
//     this->radius = radius;
// }


// bool CircleBoundingBox::isIntersecting(BoundingBox *bb) {
//     if (CircleBoundingBox* cbb = dynamic_cast<CircleBoundingBox*>(bb)) {
//         glm::vec3 circlePos1 = obj->getPosition();
//         glm::vec3 circlePos2 = cbb->obj->getPosition();
//         float distance = sqrt((circlePos1.x - circlePos2.x) * (circlePos1.x - circlePos2.x) +
//                           (circlePos1.z - circlePos2.z) * (circlePos1.z - circlePos2.z));
//         return distance < (radius + cbb->radius);
    
//     } 
//     // else if (RectangleBoundingBox* rbb = dynamic_cast<RectangleBoundingBox*>(bb)) {
//     //     return this->isIntersecting(rbb);
//     // }

//     return false;
// }

// // CircleBoundingBox::CircleBoundingBox(BoundingBox)

// // bool CircleBoundingBox::isIntersecting(RectangleBoundingBox *rbb) {
// //    return rbb->isIntersecting(this);
// // }

// // bool CircleBoundingBox::isIntersecting(CircleBoundingBox *cbb) {
// //    glm::vec3 circlePos1 = obj->getPosition();
// //    glm::vec3 circlePos2 = cbb->obj->getPosition();
// //    float distance = sqrt((circlePos1.x - circlePos2.x) * (circlePos1.x - circlePos2.x) +
// //                       (circlePos1.z - circlePos2.z) * (circlePos1.z - circlePos2.z));
// //    return distance < (radius + cbb->radius);
// // }