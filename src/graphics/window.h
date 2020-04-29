#include <glad/glad.h>		// Include this...
#include <GLFW/glfw3.h>		// ...before this.
#include "GameObject.cpp"
#include <graphics/_options/graphics_vars.h>
#include <unordered_map>

class Window
{
public:
    Window(int width, int height);

    std::unordered_map<unsigned int, GameObject> objectsToRender;
    void render();

    /**
     * @param object the game object to add to the map of objects
     * */
    void addObject(unsigned int id, GameObject object);
    
    void removeObject(unsigned int index);

private:
    Shader* shader;
    void setupWindow();
    GLFWwindow* window;
    unsigned int objNum;        // Increment when get response back from server
    int width;
    int height;
};