#include <objects/Ingredient.h>

Ingredient::Ingredient(int ID) : GameObject(ID) {
    this->objType = INGREDIENT;
    this->baseRadius = Config::getFloat("Ingredient_Radius");
	this->setModel(Config::get("Wall_Model"));
	this->setPosition(glm::vec3(1,0,1));
}

Ingredient::Ingredient() : GameObject() {
    this->objType = INGREDIENT;
    this->baseRadius = Config::getFloat("Ingredient_Radius");
	this->setModel(Config::get("Wall_Model"));
}
    
Ingredient::~Ingredient() {
    if (this->textureID != 0) {
        this->textureID = 0;
    }
}

void Ingredient::setName(std::string name) {
    this->name = name;
}

std::string Ingredient::getName() {
    return this->name;
}

void Ingredient::setQualityIndex(int i) {
    this->qualityIndex = i;
}

int Ingredient::getQualityIndex() {
    return this->qualityIndex;
}

void Ingredient::randomizeQualityIndex()
{
    // Generate a number from 1 - 100
    int randNum = rand() % 100 + 1;

    // Bad quality ingredients
    if (randNum <= Config::getInt("Bad_Quality_Prob"))
    {
        this->qualityIndex = BAD_QUALITY;
    }

    // Medium quality ingredients
    else if (randNum <= Config::getInt("Ok_Quality_Prob") + Config::getInt("Bad_Quality_Prob"))
    {
        this->qualityIndex = OK_QUALITY;
    }

    // Good quality ingredients
    else
    {
        this->qualityIndex = GOOD_QUALITY;
    }
}

IngredientStatus Ingredient::getStatus() {
    return this->status;
}

std::string Ingredient::getStringStatus() {
    return IngredientStatusToString[this->status];
}

void Ingredient::setStatus(IngredientStatus status) {
    this->status = status;
}

void Ingredient::setStatusFromString(std::string status) {
    this->status = stringToIngredientStatus[status];
}

void Ingredient::draw() {
}

GLuint Ingredient::getTextureID() {
    return this->textureID;
}

std::string Ingredient::getDetailedName() {
    std::string qualityMsg;
    switch( qualityIndex ) {
        case BAD_QUALITY : qualityMsg = "Moldy "; break;
        case OK_QUALITY : qualityMsg = "Mildly Adequate "; break;
        case GOOD_QUALITY : qualityMsg = "Soviet Union Approved "; break;
    }
    return IngredientStatusToString[getStatus()] + " " + qualityMsg + this->name;
}

std::unordered_map<IngredientStatus, std::string> Ingredient::IngredientStatusToString = {
    {IngredientStatus::Raw, "Raw"},
    {IngredientStatus::Cut, "Cut"},
    {IngredientStatus::Boiled, "Boiled"},
    {IngredientStatus::Fried, "Fried"}
};

std::unordered_map<std::string, IngredientStatus> Ingredient::stringToIngredientStatus = {
    {"Raw", IngredientStatus::Raw},
    {"Cut", IngredientStatus::Cut},
    {"Boiled", IngredientStatus::Boiled},
    {"Fried", IngredientStatus::Fried}
};