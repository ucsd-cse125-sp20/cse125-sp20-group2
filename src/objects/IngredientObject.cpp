#include <objects/IngredientObject.h>

IngredientObject::IngredientObject(int ID) : GameObject(ID) {
    this->objType = INGREDIENT;
}
    
void IngredientObject::setName(std::string name) {
    this->name = name;
};

void IngredientObject::setStatus(IngredientStatus status) {
    this->status = status;
}

std::string IngredientObject::getName() {
    return this->name;
};

IngredientStatus IngredientObject::getStatus() {
    return this->status;
}