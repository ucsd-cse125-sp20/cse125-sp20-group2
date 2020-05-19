#pragma once

// Config
#include <util/Config.h>

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
#include <constants/graphics_vars.h>

// Objects
#include <objects/GameObject.h>
#include <objects/Player.h>
#include <objects/IngredientObject.h>

#include <gui/imgui_impl_glfw.h>
#include <gui/imgui_impl_opengl3.h>

class Window
{
public:
    bool isClosed;
    Window(int width, int height);

    std::unordered_map<unsigned int, GameObject*> objectsToRender;
    std::unordered_map<int, IngredientObject*>* inventory;
    void render();

    /**
     * @param object the game object to add to the map of objects
     * */
    void addObject(unsigned int id, GameObject* object);
    
    void removeObject(unsigned int index);

    void addInventory(std::unordered_map<int, IngredientObject*>* inventory);

    void close();
    GLFWwindow* glfwViewport;
    Player* player;     // FIXME - IN THE FUTURE, WINDOW SHOULD NOT KNOW ABOUT THE PLAYER
    Camera* camera;
private:
    Shader* shader;
    Shader* UIshader;
    void setupWindow();
    void cameraViewUpdate();
    unsigned int objNum;        // Increment when get response back from server
    int width;
    int height;
};