#include <objects/Cookware.h>

Cookware::Cookware(int ID) : GameObject(ID) {
    this->setModel(Config::get("Pan_Model"));
    this->objType = COOKWARE;
    this->box = new BoundingBox(this);
    this->box->setCircleBoundingBox();
    this->baseRadius = Config::getFloat("Cookware_Radius");
    this->applyScale(Config::getVec3("Cookware_Scaling"));
}

Cookware::Cookware() : GameObject() {
    this->setModel(Config::get("Pan_Model"));
    this->objType = COOKWARE;
    this->box = new BoundingBox(this);
    this->box->setCircleBoundingBox();
    this->baseRadius = Config::getFloat("Cookware_Radius");
    this->applyScale(Config::getVec3("Cookware_Scaling"));
}

Cookware::Cookware(std::string model): GameObject() {
    this->setName(model);
    this->setModel(Config::get(model + "_Model"));
    this->objType = COOKWARE;
    this->box = new BoundingBox(this);
    this->box->setCircleBoundingBox();
    this->baseRadius = Config::getFloat("Cookware_Radius");
    this->applyScale(Config::getVec3("Cookware_Scaling"));
}

void Cookware::setBusy(bool busy) {

    // Set models based on busy or not
    if (busy)
    {
        if (name == PAN)                this->setModel(Config::get("Oil_Pan_Model"));
        else if (name == POT)           this->setModel(Config::get("Water_Pot_Model"));
        else if (name == CUTTING_BOARD) this->setModel(Config::get("Tomato_Cutting_Board_Model"));
    }
    else 
    {
        if (name == PAN)                this->setModel(Config::get("Pan_Model"));
        else if (name == POT)           this->setModel(Config::get("Pot_Model"));
        else if (name == CUTTING_BOARD) this->setModel(Config::get("Cutting_Board_Model"));
    }

    // set
    this->busy = busy;
}

bool Cookware::getBusy() {
    return this->busy;
}
    
void Cookware::setName(std::string name) {
    this->name = name;
}

std::string Cookware::getName() {
    return this->name;
}

bool Cookware::cookIngredient(Ingredient* ing) {
    if( this->name == PAN ) {
        if( ing->getStatus() == IngredientStatus::Boiled || ing->getStatus() == IngredientStatus::Fried )
            return false;
        else
            ing->setStatus(IngredientStatus::Fried);
    } else if ( this->name == POT ) {
        if( ing->getStatus() == IngredientStatus::Boiled || ing->getStatus() == IngredientStatus::Fried )
            return false;
        else
            ing->setStatus(IngredientStatus::Boiled);
    } else if ( this->name == CUTTING_BOARD ) {
        if( ing->getStatus() == IngredientStatus::Boiled || ing->getStatus() == IngredientStatus::Fried || ing->getStatus() == IngredientStatus::Cut )
            return false;
        else
            ing->setStatus(IngredientStatus::Cut);
    }
    return true;
}
