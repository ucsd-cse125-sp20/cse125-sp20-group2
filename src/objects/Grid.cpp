#include <objects/Grid.h>
#include <objects/Tile.h>

Grid::Grid(int ID, int x, int z) : GameObject(ID) {
    this->x = x;
    this->z = z;
    this->createTiles();
}

void Grid::createTiles() {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < z; j++) {

            // TODO: HARDCODED VALUE
            int ID = 99999 + i + j;

            Tile* tile = new Tile(ID);
            this->setItem(ID, tile);
            tile->setPosition(this->getPosition() + glm::vec3(i, 0, j));
        }
    }
}