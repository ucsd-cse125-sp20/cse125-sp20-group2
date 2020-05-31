#pragma once

#include <objects/GameObject.h>
#include <objects/Ingredient.h>
#include <string>

class Cookware : public GameObject 
{
protected:
    std::string name;

    bool busy;

public:
    Cookware(int ID);

    Cookware();

    Cookware(std::string name);

    void setBusy(bool busy);

    bool getBusy();

    void setName(std::string name);

    bool cookIngredient(Ingredient* ing);

    std::string getName();
};