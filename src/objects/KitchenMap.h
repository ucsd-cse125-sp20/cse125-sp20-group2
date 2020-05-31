#pragma once

#include <objects/Map.h>
#include <objects/Cookware.h>
#include <objects/Table.h>
#include <objects/Plate.h>

class KitchenMap : public Map {
    public:
        KitchenMap();

        std::vector<Cookware*> cookwareObjects;
        std::vector<Table*> tableList;
        std::vector<Plate*> plateObjects;

        void toggleRender();
        void setRender(bool render);
};