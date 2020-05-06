#pragma once

#include <objects/GameObject.h>
#include <string>

enum class IngredientStatus
{
    Raw,
    Cut,
    Cooked
};

class IngredientObject : public GameObject
{
protected:
    std::string name;

    IngredientStatus status;

    // TODO: Add inventory image
public:
    IngredientObject(int ID);

    void setName(std::string name);

    void setStatus(IngredientStatus status);

    std::string getName();

    IngredientStatus getStatus();
};