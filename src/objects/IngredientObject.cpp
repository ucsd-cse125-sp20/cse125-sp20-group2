#include <objects/IngredientObject.h>

IngredientObject::IngredientObject(int ID) : GameObject(ID) {
    this->objType = INGREDIENT;
	this->setModel(Config::get("Wall_Model"));
	this->setPosition(glm::vec3(1,0,1));
    
}

IngredientObject::IngredientObject() : GameObject() {
    this->objType = INGREDIENT;
	this->setModel(Config::get("Wall_Model"));
    
}
    
IngredientObject::~IngredientObject() {
    if (this->textureID != 0) {
        this->textureID = 0;
    }
}

void IngredientObject::setName(std::string name) {
    this->name = name;
};


std::string IngredientObject::getName() {
    return this->name;
};

IngredientStatus IngredientObject::getStatus() {
    return this->status;
}

std::string IngredientObject::getStringStatus() {
    return IngredientStatusToString[this->status];
}

void IngredientObject::setStatus(IngredientStatus status) {
    this->status = status;
}

void IngredientObject::setStatusFromString(std::string status) {
    this->status = stringToIngredientStatus[status];
}

void IngredientObject::draw() {
}

GLuint IngredientObject::getTextureID() {
    return this->textureID;
}

std::unordered_map<IngredientStatus, std::string> IngredientObject::IngredientStatusToString = {
    {IngredientStatus::Raw, "Raw"},
    {IngredientStatus::Cut, "Cut"},
    {IngredientStatus::Cooked, "Cooked"}
};

std::unordered_map<std::string, IngredientStatus> IngredientObject::stringToIngredientStatus = {
    {"Raw", IngredientStatus::Raw},
    {"Cut", IngredientStatus::Cut},
    {"Cooked", IngredientStatus::Cooked}
};