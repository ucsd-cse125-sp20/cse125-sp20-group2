#pragma once

#include <objects/GameObject.h>
#include <objects/Ingredient.h>
#include <stack>

class Plate : public GameObject
{
    private:
        // The client who owns the plates
        int clientId;

        // The contents of the plate
        std::stack<Ingredient *> plateContents;

    public:
        Plate();
        Plate(int ID);
        ~Plate();

        // Used for ingredient offset
        float ingredientOffset;

        /*
        * Adds the specified ingredient to the plate
        * */
        void addToPlate(Ingredient *ingredient);

        /*
        * Removes the top ingredient on the plate
        * */
        Ingredient *removeFromPlate();

        /*
        * Assign a client Id to a plate
        * */
       void assignId(int clientId);

       int getClientId();
};
