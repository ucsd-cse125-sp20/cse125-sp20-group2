#include <objects/GameObject.h>


GameObject::GameObject(int id) {
	this->ID = id;
	this->setPosition(glm::vec3(0, 0, 0));
	this->setRotation(0);
	this->applyScale(glm::vec3(0.2));
	this->setPassable(false);
}

void GameObject::draw(Shader shader) {
	this->model->draw(shader);
}

int GameObject::getID() {
	return this->ID;
}

void GameObject::setModel(std::string path) {
	this->model = new Model(path);
}

// Update the world position and move the model matrix
void GameObject::setPosition(glm::vec3 loc) {
	this->worldPos = loc;
}

// Apply scaling to the model.
void GameObject::applyScale(glm::vec3 scale) {
	this->scaleVec = scale;
}

glm::vec3 GameObject::getWorldPos() {
	return this->worldPos;
}

float GameObject::getRotation() {
	return this->rotation;
}

// Set rotation and rotate the model to the new angle.
void GameObject::setRotation(float rot) {
	this->rotation = rot;
}

// Will probably be used on item pickup in dungeon phase
void GameObject::setItem(int index, GameObject* item) {
	this->inventory[index] = item;
}

GameObject* GameObject::getItem(int index) {
	return this->inventory[index];
}

// Will probably be used on item placement in cooking phase
void GameObject::removeItem(int index) {
    this->inventory.erase(index);
}

bool GameObject::isPassable() {
	return this->passable;
}

void GameObject::setPassable(bool passable) {
	this->passable = passable;
}

glm::vec3 GameObject::getScaleVec() {
    return this->scaleVec;
}