#include <objects/Plate.h>

Plate::Plate() : GameObject()
{
    this->setObjectType(ObjectType::PLATE);
}

Plate::Plate(int ID) : GameObject(ID)
{
    this->setObjectType(ObjectType::PLATE);
}

Plate::~Plate()
{
}

void Plate::addToPlate(Ingredient *ingredient)
{
    this->plateContents.push(ingredient);
}

Ingredient *Plate::removeFromPlate()
{
    Ingredient *currIngredient = this->plateContents.top();
    this->plateContents.pop();
    return currIngredient;
}