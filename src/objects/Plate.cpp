#include <objects/Plate.h>

Plate::Plate() : GameObject()
{
    this->setModel(Config::get("Plate_Model"));
    this->setObjectType(ObjectType::PLATE);
    this->box = new BoundingBox(this);
    this->box->setRadius(0.5);
    this->applyScale(Config::getVec3("Plate_Scaling"));
    this->ingredientOffset = 2;
}

Plate::Plate(int ID) : GameObject(ID)
{
    this->setModel(Config::get("Plate_Model"));
    this->setObjectType(ObjectType::PLATE);
    this->box = new BoundingBox(this);
    this->box->setRadius(0.5);
    this->applyScale(Config::getVec3("Plate_Scaling"));
    this->ingredientOffset = 2;
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

void Plate::assignId(int clientId)
{
    this->clientId = clientId; 
}

int Plate::getClientId()
{
    return this->clientId;
}