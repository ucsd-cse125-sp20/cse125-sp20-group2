#include <objects/CookwareObject.h>

CookwareObject::CookwareObject(int ID) : GameObject(ID) {
    this->objType = COOKWARE;
}

CookwareObject::CookwareObject() : GameObject() {
    this->objType = COOKWARE;
	this->setModel(Config::get("Wall_Model"));
    
}
    
void CookwareObject::setName(std::string name) {
    this->name = name;
};


std::string CookwareObject::getName() {
    return this->name;
};
