#include <objects/Wall.h>

Wall::Wall() : GameObject() {
	this->setModel(Config::get("Wall_Model"));
    this->getBoundingBox()->setDepth(2);
    this->getBoundingBox()->setWidth(3);
    this->applyScale(glm::vec3(3, 2, 2));
    this->objType = WALL;
}

Wall::Wall(int id) : GameObject(id) {
	this->setModel(Config::get("Wall_Model"));
    this->getBoundingBox()->setDepth(2);
    this->getBoundingBox()->setWidth(3);
    this->applyScale(glm::vec3(3, 2, 2));
    this->objType = WALL;
}