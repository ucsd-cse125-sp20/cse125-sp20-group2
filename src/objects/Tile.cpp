#include <objects/Tile.h>

Tile::Tile() : GameObject() {
	this->setModel(Config::get("Tile_Model"));
    this->applyScale(Config::getVec3("Wall_Scaling"));
    this->objType = OBJECT;
    this->setPassable(true);
}

Tile::Tile(int ID) : GameObject(ID) {
    this->setModel(Config::get("Tile_Model"));
    this->applyScale(Config::getVec3("Wall_Scaling"));
    this->objType = OBJECT;
    this->setPassable(true);
}