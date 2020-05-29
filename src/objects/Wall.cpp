#include <objects/Wall.h>

Wall::Wall() : GameObject() {
	this->setModel(Config::get("Wall_Model"));
    this->applyScale(Config::getVec3("Wall_Scaling"));
    this->objType = WALL;
}

Wall::Wall(int id) : GameObject(id) {
	this->setModel(Config::get("Wall_Model"));
    this->applyScale(Config::getVec3("Wall_Scaling"));
    this->objType = WALL;
}