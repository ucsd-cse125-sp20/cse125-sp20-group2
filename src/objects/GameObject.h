#pragma once

#include <graphics/_obj/Shader.h>
#include <graphics/Model.cpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/_options/graphics_vars.h>
#include <unordered_map>

class GameObject
{
private:
    // Rendered model
    Model* model;

    // Position in the world
	glm::vec3 worldPos;

    // Used to determine transformations of object
	glm::vec3 scaleVec;
    float rotation;

    // Identifies the rendering ID of the object
	int ID;

    // Inventory
    std::unordered_map<unsigned int, GameObject*> inventory;

    // Passing through object
    bool passable = false;

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

    int getID();

    /** TODO: change to getPosition ...
     * Get the world position of the object
     * */
    glm::vec3 getWorldPos();

    /**
     * Moves the object and sets the world position.
     * 
     * @param loc - The new location of the object.
     * */
    void setPosition(glm::vec3 loc);

    /**
     * The scale vector (x, y, z scaling) to be applied to the object.
     * 
     * @param scale - The scale vector to be applied.
     * */
    void applyScale(glm::vec3 scale);

    glm::vec3 getScaleVec();

    float getRotation();

    void setRotation(float rot);
    
    GameObject* getItem(int index);
    
    // Will probably be used on item pickup in dungeon phase
    void setItem(int index, GameObject* item);

    // Will probably be used on item placement in cooking phase
    void removeItem(int index);

    bool isPassable();

    void setPassable(bool passable);
};