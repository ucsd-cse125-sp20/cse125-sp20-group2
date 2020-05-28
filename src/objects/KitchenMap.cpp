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
    std::cout << "we out here" << std::endl;

    std::cout << "accessing wallist" << std::endl;
    auto wallsxd = this->wallList;
    std::cout << "for loop" << std::endl;

    for(auto & wall : this->wallList)
    {
        std::cout << "in the for loop" << std::endl;
        if (wall == NULL)
        {
            std::cout << "why the wall null xd" << std::endl;
        }
        wall->setRender(render);
    }

    for(auto table : this->tableList)
        table->setRender(render);

    for(auto c : this->cookwareObjects)
        c->setRender(render);
}