#pragma once

#include <objects/GameObject.h>

enum class IngredientStatus
{
    Raw,
    Cut,
    Cooked
};

class IngredientObject : public GameObject
{
protected:
    String name;

    IngredientStatus status;

    // TODO: Add inventory image
public:
    IngredientObject(int ID);

    void setName(string name);

    void setStatus(IngredientStatus status);

    string getName();

    IngredientStatus getStatus();
};