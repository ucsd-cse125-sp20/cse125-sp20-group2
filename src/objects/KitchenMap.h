#include <objects/Map.h>
#include <objects/CookwareObject.h>

class KitchenMap : public Map {
    public:
        std::vector<CookwareObject*> cookwareObjects;
}