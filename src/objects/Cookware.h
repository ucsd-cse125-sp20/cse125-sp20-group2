#pragma once

#include <objects/GameObject.h>
#include <objects/Ingredient.h>
#include <string>

const std::string POT = "Pot";
const std::string PAN = "Pan";
const std::string CUTTING_BOARD = "Cutting Board";

class Cookware : public GameObject 
{
protected:
    std::string name;

    bool busy;

public:
    Cookware(int ID);

    Cookware();

    void setBusy(bool busy);

    bool getBusy();

    void setName(std::string name);

    bool cookIngredient(Ingredient* ing);

    std::string getName();
};