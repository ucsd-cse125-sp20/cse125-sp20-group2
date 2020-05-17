#include <objects/Wall.h>

Wall::Wall() : GameObject() {
    ///TODO: Wall sizing should not be hardcoded.
	this->setModel(Config::get("Wall_Model"));
    this->applyScale(glm::vec3(3, 2, 2));
    this->objType = WALL;
}

Wall::Wall(int id) : GameObject(id) {
	this->setModel(Config::get("Wall_Model"));
    this->applyScale(glm::vec3(3, 2, 2));
    this->objType = WALL;
}