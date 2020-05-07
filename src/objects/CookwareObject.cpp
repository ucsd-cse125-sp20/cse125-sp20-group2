#include <objects/CookwareObject.h>

CookwareObject::CookwareObject(int ID) : GameObject(ID) {
    this->objType = COOKWARE;
}
    
void CookwareObject::setName(std::string name) {
    this->name = name;
};


std::string CookwareObject::getName() {
    return this->name;
};
