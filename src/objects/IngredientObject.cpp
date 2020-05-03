#include <objects/IngredientObject.h>

IngredientObject::IngredientObject(int ID) : GameObject(ID) {

}
    
void IngredientObject::setName(String name) {
    this->name = name;
};

void IngredientObject::setStatus(IngredientStatus status) {
    this->status = status;
}

string IngredientObject::getName() {
    return this->name;
};

IngredientStatus IngredientObject::setStatus() {
    return this->status;
}