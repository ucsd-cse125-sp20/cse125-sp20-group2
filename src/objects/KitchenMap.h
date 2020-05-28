#pragma once

#include <objects/Map.h>
#include <objects/Cookware.h>
#include <objects/Table.h>

class KitchenMap : public Map {
    public:
        KitchenMap();

        std::vector<Cookware*> cookwareObjects;
        std::vector<Table*> tableList;

        void toggleRender();
        void setRender(bool render);
};