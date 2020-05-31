#include <objects/DungeonMap.h>

void DungeonMap::setRender(bool b) {
    for(auto wall : this->wallList)
        wall->setRender(b);
}