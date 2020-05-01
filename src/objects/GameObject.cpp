#include <objects/GameObject.h>

GameObject::GameObject(int id) {
	this->ID = id;
}

void GameObject::draw(Shader shader) {
	this->model->draw(shader);
}

void GameObject::setID(int ID) {
	this->ID = ID;
}

int GameObject::getID() {
	return this->ID;
}

void GameObject::setModel(std::string path) {
	this->model = new Model(path);
}

// Update the world position and move the model matrix
void GameObject::moveTo(glm::vec3 loc) {
	this->worldPos = loc;
	this->modelMatrix = glm::translate(modelMatrix, loc);
}

// Apply scaling to the model.
void GameObject::applyScale(glm::vec3 scale) {
	this->scaleVec = scale;
	this->modelMatrix = glm::scale(modelMatrix, scale);
}

// Rotate the model.
void GameObject::rotate(float rotation, glm::vec3 axis = UP) {
	this->modelMatrix = glm::rotate(modelMatrix, rotation, axis);
}

glm::vec3 GameObject::getWorldPos() {
	return this->worldPos;
}

glm::mat4 GameObject::getModelMatrix() {
	return this->modelMatrix;
}

glm::mat4 GameObject::getNormalMatrix() {
	return glm::mat3(glm::transpose(glm::inverse(this->modelMatrix)));
}

float GameObject::getRotation() {
	return this->rotation;
}

// Set rotation and rotate the model to the new angle.
void GameObject::setRotation(float rot) {
	this->rotation = rot;
}