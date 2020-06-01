#include <graphics/render/Model.h>

class Animation
{
public:
    Model* frames;

    Animation(std::string animName);

    void play(Player* player);
};