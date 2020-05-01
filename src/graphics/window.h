#include <glad/glad.h>		// Include this...
#include <GLFW/glfw3.h>		// ...before this.
#include <objects/GameObject.h>
#include <graphics/_options/graphics_vars.h>
#include <unordered_map>
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
    void processInput();
private:
    Shader* shader;
    void setupWindow();
    
    unsigned int objNum;        // Increment when get response back from server
    int width;
    int height;
};