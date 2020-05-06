#pragma once

// OpenGL function pointers
#include <glad/glad.h>		// Include this...
#include <GLFW/glfw3.h>		// ...before this.

// Std library packages
#include <iostream>
#include <unordered_map>

// ASSIMP
#include <assimp/config.h>

// Matrix operations and values
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

// Rendering classes
#include <graphics/render/Mesh.h>
#include <graphics/render/Shader.h>
#include <graphics/render/Camera.h>
#include <graphics/render/Model.h>

// Etc
#include <config/graphics_vars.h>

// Objects
#include <objects/GameObject.h>
#include <objects/Player.h>

class Window
{
public:
    bool isClosed;
    Window(int width, int height);

    std::unordered_map<unsigned int, GameObject*> objectsToRender;
    void render();

    /**
     * @param object the game object to add to the map of objects
     * */
    void addObject(unsigned int id, GameObject* object);
    
    void removeObject(unsigned int index);

    void close();
    GLFWwindow* glfwViewport;
    Player* player;     // FIXME - IN THE FUTURE, WINDOW SHOULD NOT KNOW ABOUT THE PLAYER
private:
    Shader* shader;
    void setupWindow();
    
    unsigned int objNum;        // Increment when get response back from server
    int width;
    int height;
};