#pragma once

#include <objects/GameObject.h>
#include <string>

class Cookware : public GameObject 
{
protected:
    std::string name;

public:
    Cookware(int ID);

    Cookware();

    void setName(std::string name);

    std::string getName();
};