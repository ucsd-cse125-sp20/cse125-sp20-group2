#include "GameObject.cpp"
#include "graphics_vars.h"
#include <unordered_map>

class Window
{
public:
    Window(int width, int height);

    std::unordered_map<unsigned int, GameObject> objectsToRender;

    int render();

    /**
     * @param object the game object to add to the map of objects
     * @return boolean indicating whether or not object was successfully added
     * */
    bool addObject(unsigned int, GameObject object);
    
    bool removeObject(unsigned int index);

private:
    // Increment when get response back from server
    unsigned int objNum;

    int width;
    
    int height;
};