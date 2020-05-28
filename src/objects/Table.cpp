#include <objects/Table.h>

Table::Table() : GameObject() {
    ///TODO: Wall sizing should not be hardcoded.
    // TODO: Change to dedicated wall model
	this->setModel(Config::get("Wall_Model"));
    this->applyScale(glm::vec3(2, 2, 1));
    this->objType = TABLE;
}

Table::Table(int id) : GameObject(id) {
	this->setModel(Config::get("Wall_Model"));
    this->applyScale(glm::vec3(2, 2, 1));
    this->objType = TABLE;
}