#include <objects/Map.h>

class DungeonMap : public Map {
    public:
        std::vector<glm::vec3> ingredientPositions;
}