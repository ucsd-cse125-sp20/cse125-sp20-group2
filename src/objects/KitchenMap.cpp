#include <objects/KitchenMap.h>

KitchenMap::KitchenMap() {

}

void KitchenMap::toggleRender() {
    for(auto wall : this->wallList)
        wall->toggleRender();

    for(auto table : this->tableList)
        table->toggleRender();

    for(auto c : this->cookwareObjects)
        c->toggleRender();
}

void KitchenMap::setRender(bool render) {
    for(auto & wall : this->wallList)
        wall->setRender(render);

    for(auto table : this->tableList)
        table->setRender(render);

    for(auto c : this->cookwareObjects)
        c->setRender(render);
}