#include <objects/GameObject.h>

/// TODO: remove this later
int GameObject::counter = 1000000;

/// TODO: Remove this later
GameObject::GameObject() {
	this->ID = GameObject::counter;
	GameObject::counter++;
	this->setPosition(glm::vec3(0, 0, 0));
	this->setRotation(0);
	this->applyScale(glm::vec3(0.2));
	this->model = NULL;
	this->box = new BoundingBox(this); 
	this->setPassable(false);
	this->setObjectType(OBJECT);
}

/**
 * Initialize a new gameobject with a unique ID that should be provided by the server.
 * 
 * @param id - The unique unsigned int identifier of this object.
 * */
GameObject::GameObject(int id) {

	// Set id
	this->ID = id;

	// Set transformations, assuming origin with no rotation
	this->setPosition(glm::vec3(0, 0, 0));
	this->setRotation(0);
	this->applyScale(glm::vec3(0.2));

	// Model should be set afterward
	this->model = NULL;

	// For collisions
	this->box = new BoundingBox(this); 
	this->setPassable(false);

	// Generic object type
	this->setObjectType(OBJECT);
}

/**GameObject:~GameObject() {
	if (model) delete model;
	if (box) delete box;
}*/

void GameObject::setObjectType(ObjectType newObjType) {
	this->objType = newObjType;
}

ObjectType GameObject::getObjectType() {
	return this->objType;
}

void GameObject::draw(Shader shader) {
	this->model->draw(shader);
}

int GameObject::getID() {
	return this->ID;
}

BoundingBox* GameObject::getBoundingBox() {
	return box;
}

void GameObject::setModel(std::string path) {
	this->modelPath = path;
	this->model = new Model(path);
	this->updateMeasurements();
}

/// TODO: Consider if height is needed
void GameObject::updateMeasurements()
{
	// This code serves to update collisions. If either of these are missing, stop
	if (!model || !box) return;

	// Update model
	this->width = model->modelWidth * scaleVec.x;
	this->depth = model->modelDepth * scaleVec.z;

	// Update collision box, if applicable
	if (!this->box->isCircleBoundingBox())
	{
		this->box->setWidth(width);
		this->box->setDepth(depth);
		this->box->updateCorners();
	} 

	// Update collision circle, if applicable
	else 
	{
		this->box->setRadius(width/2);
	}
}

std::string GameObject::getModelPath()
{
	return this->modelPath;
}

// Update the world position
void GameObject::setPosition(glm::vec3 loc) {
	this->pos = loc;
}

void GameObject::setPosition(float x, float y, float z) {
	this->pos = glm::vec3(x, y, z);
}

void GameObject::renderInvisible() {
	glm::vec3 originalPos = this->getPosition();
	glm::vec3 newPos = glm::vec3(originalPos.x, -2, originalPos.z);
	this->setPosition(newPos);
	this->render = false;
	this->box = NULL;
}

bool GameObject::getRender() {
	return this->render;
}

// toggle render
void GameObject::toggleRender() {
	this->render = !this->render;
}

// Apply scaling to the model.
void GameObject::applyScale(glm::vec3 scale) {
	this->scaleVec = scale;
	this->updateMeasurements();
}

glm::vec3 GameObject::getPosition() {
	return this->pos;
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

bool GameObject::isColliding(GameObject* obj) {
	return this->box->isIntersecting(obj->box);
}