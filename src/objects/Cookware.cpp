#include <objects/Cookware.h>

Cookware::Cookware(int ID) : GameObject(ID) {
    this->objType = COOKWARE;
}

Cookware::Cookware() : GameObject() {
    this->objType = COOKWARE;
    this->applyScale(Config::getVec3("Cookware_Scaling"));
}
    
void Cookware::setName(std::string name) {
    this->name = name;
};

std::string Cookware::getName() {
    return this->name;
};
