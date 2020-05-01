#pragma once

#include <graphics/_obj/Shader.h>
#include <graphics/Model.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/_options/graphics_vars.h>

class GameObject
{
private:
    // Rendered model
    Model* model;

    // Position in the world
	glm::vec3 worldPos;

    // Used to determine transformations of object
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::vec3 scaleVec;
    float rotation;

    // Identifies the rendering ID of the object
	int ID;

    // Inventory
    std::unordered_map<int, GameObject> inventory;

    // Passing through object
    bool passable;

public:
    /**
     * This is a constructor for both the server and client.
     * 
     * @param id - This is the id of the game object
     * */
    GameObject(int id);

    /**
     * Sets the model of the object based on a given string.
     * 
     * @param path - This is the file path to the model.
     * */
    void setModel(std::string path);

    /**
     * Draw the model using the given shader.
     * 
     * @param shader - The shader to use to draw the model.
     * */
    void draw(Shader shader);

    /**
     * Sets the rendering ID to be used by a Window object.
     * 
     * @param ID - The rendering ID identifying the object.
     * */
    void setID(int ID);

    int getID();

    /**
     * Get the world position of the object
     * */
    glm::vec3 getWorldPos();

    /**
     * Moves the object and sets the world position.
     * 
     * @param loc - The new location of the object.
     * */
    void moveTo(glm::vec3 loc);

    /**
     * The scale vector (x, y, z scaling) to be applied to the object.
     * 
     * @param scale - The scale vector to be applied.
     * */
    void applyScale(glm::vec3 scale);

    /**
     * The rotation angle and axis to be applied to the object. Default
     * is y-axis.
     * 
     * @param rotation - The angle of rotation to be applied.
     * @param axis - The 3D axis to rotate the object on.
     * */
    void rotate(float rotation, glm::vec3 axis);

    /**
     * Get the model matrix
     * */
    glm::mat4 getModelMatrix();

    /**
     * Get the normal matrix
     * */
    glm::mat4 getNormalMatrix();

    float getRotation();

    void setRotation(float rot);
    
    // Will probably be used on item pickup in dungeon phase
    void addItem(int index, GameObject item);

    // Will probably be used on item placement in cooking phase
    void removeItem(int index);

    // Will probably be used on item prep in cooking phase
    void replaceItem(int index, GameObject item);

    bool isPassable();

    void setPassable(bool passable);
};