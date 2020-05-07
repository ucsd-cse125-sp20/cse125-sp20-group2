#pragma once

#include <objects/GameObject.h>
#include <string>

class CookwareObject : public GameObject 
{
protected:
    std::string name;

public:
    CookwareObject(int ID);

    void setName(std::string name);

    std::string getName();
};