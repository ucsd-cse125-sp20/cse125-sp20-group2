#include <util/MapBuilder.h>

Map* MapBuilder::getBasicMap() {
    Wall* wall = new Wall();
    wall->setPosition(glm::vec3(0, 0, 5));
    Map *mp = new Map();
    mp->wallList.push_back(wall);
    return mp;
}