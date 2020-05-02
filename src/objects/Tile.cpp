#include <objects/Tile.h>

Tile::Tile(int ID) : GameObject(ID) {
    this->setPassable(true);
}