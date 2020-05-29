#include <objects/Table.h>

Table::Table() : GameObject() {
	this->setModel(Config::get("Table_Model"));
    this->applyScale(Config::getVec3("Table_Scaling"));
    this->objType = TABLE;
}

Table::Table(int id) : GameObject(id) {
	this->setModel(Config::get("Wall_Model"));
    this->applyScale(Config::getVec3("Table_Scaling"));
    this->objType = TABLE;
}